#include "raylib.h"
#include "monsters/monster.h"
#include "monsters/combat.h"
#include <math.h>
#include <stdio.h>

#define MAX_TEST_PROJECTILES 30

typedef struct {
    bool active;
    Vector2 position;
    Vector2 velocity;
    float radius;
    float damage;
} TestProjectile;

int main(void) {
    const int screenWidth = 1000;
    const int screenHeight = 700;
    InitWindow(screenWidth, screenHeight, "Monster & Boss Integration Test");
    SetTargetFPS(60);

    Monster monsters[MAX_MONSTERS];
    Boss boss;
    InitMonsters(monsters);
    InitBoss(&boss);

    TestProjectile projectiles[MAX_TEST_PROJECTILES] = { 0 };

    Vector2 playerPos = { screenWidth / 2.0f, screenHeight / 2.0f };
    float playerRadius = 18.0f;
    float playerHp = 500.0f;
    float playerSpeed = 200.0f;

    int currentStage = 1;
    int totalExp = 0;
    int killCount = 0;
    float spawnTimer = 0.0f;
    char lastEventLog[160] = "Controls: [WASD] Move | [Click] Auto-Aim Shoot | [1] Midterm | [2] Final | [K] Kill Boss | [E] Stage+";

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // 1. 플레이어 이동 (WASD)
        if (IsKeyDown(KEY_W)) playerPos.y -= playerSpeed * dt;
        if (IsKeyDown(KEY_S)) playerPos.y += playerSpeed * dt;
        if (IsKeyDown(KEY_A)) playerPos.x -= playerSpeed * dt;
        if (IsKeyDown(KEY_D)) playerPos.x += playerSpeed * dt;

        // 2. 스테이지 증가 테스트 ([E] 키)
        if (IsKeyPressed(KEY_E)) {
            currentStage++;
            snprintf(lastEventLog, sizeof(lastEventLog), "[STAGE UP] Current Stage: %d (Monster Bonus HP: +%d)",
                     currentStage, (currentStage - 1) * STAGE_HP_INCREMENT);
        }

        // 3. 보스 개별 생성 및 중복 방지 검증 ([1]: 중간고사 보스, [2]: 기말고사 보스)
        if (IsKeyPressed(KEY_ONE)) {
            bool wasActive = boss.active;
            SpawnMidtermBoss(&boss); // 내부 중복 방지 동작 확인
            if (wasActive) {
                snprintf(lastEventLog, sizeof(lastEventLog), "[DUPLICATE BLOCKED] Boss is already active! (HP: %.0f)", boss.hp);
            } else {
                snprintf(lastEventLog, sizeof(lastEventLog), "[SPAWN] Midterm Boss spawned! (HP: %.0f, Radius: %.0f)", boss.hp, boss.collisionRadius);
            }
        }
        if (IsKeyPressed(KEY_TWO)) {
            bool wasActive = boss.active;
            SpawnFinalBoss(&boss); // 내부 중복 방지 동작 확인
            if (wasActive) {
                snprintf(lastEventLog, sizeof(lastEventLog), "[DUPLICATE BLOCKED] Boss is already active! (HP: %.0f)", boss.hp);
            } else {
                snprintf(lastEventLog, sizeof(lastEventLog), "[SPAWN] Final Boss spawned! (HP: %.0f, Radius: %.0f)", boss.hp, boss.collisionRadius);
            }
        }

        // 4. 보스 처치 및 App 결과 전달 검증 ([K] 키: 보스에게 1000 대미지)
        if (IsKeyPressed(KEY_K) && boss.active) {
            MonsterHitResult res = DamageBoss(&boss, 1000.0f);
            if (res.killed) {
                totalExp += res.expReward;
                killCount++;
                snprintf(lastEventLog, sizeof(lastEventLog), "[APP RESULT] BOSS KILLED! isBoss=%d, BossType=%s, EXP=+%d",
                         res.isBoss, (res.bossType == BOSS_MIDTERM) ? "MIDTERM" : "FINAL", res.expReward);
            } else {
                snprintf(lastEventLog, sizeof(lastEventLog), "[BOSS HIT] Boss HP remaining: %.0f / %.0f", boss.hp, boss.maxHp);
            }
        }

        // 5. 일반 몬스터 3종 주기적 스폰
        spawnTimer += dt;
        if (spawnTimer >= SPAWN_INTERVAL) {
            spawnTimer = 0.0f;
            SpawnRandomMonster(monsters, currentStage);
        }

        // 6. 최단 거리 몬스터 탐색 + 투사체 발사 (마우스 좌클릭)
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 targetPos = GetMousePosition();
            FindNearestMonsterPosition(monsters, &boss, playerPos, &targetPos);

            float dx = targetPos.x - playerPos.x;
            float dy = targetPos.y - playerPos.y;
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist > 0.001f) {
                for (int i = 0; i < MAX_TEST_PROJECTILES; i++) {
                    if (!projectiles[i].active) {
                        projectiles[i].active = true;
                        projectiles[i].position = playerPos;
                        projectiles[i].velocity.x = (dx / dist) * 450.0f;
                        projectiles[i].velocity.y = (dy / dist) * 450.0f;
                        projectiles[i].radius = 6.0f;
                        projectiles[i].damage = 40.0f;
                        break;
                    }
                }
            }
        }

        // 7. 일반 몬스터 및 보스 이동 업데이트
        UpdateMonsters(monsters, playerPos, dt);
        UpdateBoss(&boss, playerPos, dt);

        // 8. 플레이어 vs 일반 몬스터 + 보스 접촉 충돌 피해 처리
        float takenDamage = ResolvePlayerMonsterCombat(monsters, &boss, playerPos, playerRadius);
        if (takenDamage > 0.0f) {
            playerHp -= takenDamage;
            snprintf(lastEventLog, sizeof(lastEventLog), "[PLAYER HIT] Took %.0f damage! (Remaining HP: %.0f)", takenDamage, playerHp);
        }

        // 9. 투사체 충돌 판정 및 사망 여부/몬스터 종류 App 전달 확인
        for (int i = 0; i < MAX_TEST_PROJECTILES; i++) {
            if (!projectiles[i].active) continue;

            projectiles[i].position.x += projectiles[i].velocity.x * dt;
            projectiles[i].position.y += projectiles[i].velocity.y * dt;

            // 화면 밖으로 나간 투사체 정리
            if (projectiles[i].position.x < -50 || projectiles[i].position.x > screenWidth + 50 ||
                projectiles[i].position.y < -50 || projectiles[i].position.y > screenHeight + 50) {
                projectiles[i].active = false;
                continue;
            }

            MonsterHitResult hitRes = ResolveProjectileMonsterCombat(
                monsters, &boss, projectiles[i].position, projectiles[i].radius, projectiles[i].damage
            );

            if (hitRes.hit) {
                projectiles[i].active = false;
                if (hitRes.killed) {
                    totalExp += hitRes.expReward;
                    killCount++;

                    if (hitRes.isBoss) {
                        snprintf(lastEventLog, sizeof(lastEventLog), "[APP RESULT] BOSS KILLED! BossType=%s, EXP=+%d",
                                 (hitRes.bossType == BOSS_MIDTERM) ? "MIDTERM" : "FINAL", hitRes.expReward);
                    } else {
                        const char* typeName = "UNKNOWN";
                        if (hitRes.monsterType == MONSTER_SLEEPY) typeName = "SLEEPY";
                        else if (hitRes.monsterType == MONSTER_POP_QUIZ) typeName = "POP_QUIZ";
                        else if (hitRes.monsterType == MONSTER_TEAM_PROJECT) typeName = "TEAM_PROJECT";

                        snprintf(lastEventLog, sizeof(lastEventLog), "[APP RESULT] MONSTER KILLED! Type=%s, EXP=+%d",
                                 typeName, hitRes.expReward);
                    }
                }
            }
        }

        // 10. 화면 렌더링
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawMonsters(monsters);
        DrawBoss(&boss);
        DrawBossHpBar(&boss, screenWidth);

        // 임시 플레이어 렌더링
        DrawCircleV(playerPos, playerRadius, BLUE);

        // 투사체 렌더링
        for (int i = 0; i < MAX_TEST_PROJECTILES; i++) {
            if (projectiles[i].active) {
                DrawCircleV(projectiles[i].position, projectiles[i].radius, DARKGREEN);
            }
        }

        // 상단 상태 및 하단 App 전달 결과 로그 표시
        DrawText(TextFormat("Stage: %d | Player HP: %.0f | EXP: %d | Kills: %d", currentStage, playerHp, totalExp, killCount), 20, 15, 20, BLACK);
        DrawText(lastEventLog, 20, screenHeight - 35, 18, DARKBLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}