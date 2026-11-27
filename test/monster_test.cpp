#include "raylib.h"
#include "monsters/monster.h"
#include "monsters/combat.h"   
#include <cmath>             

// 테스트용 투사체 구조체
struct TestProjectile {
    Vector2 position;
    Vector2 velocity;
    float radius;
    float damage;
    bool active;
};

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Exam Season Survivors - Monster & Combat Test");
    SetTargetFPS(60);

    Monster monsters[MAX_MONSTERS];
    InitMonsters(monsters);

    // 테스트용 플레이어 상태 및 투사체 배열
    Vector2 targetPos = { 640.0f, 360.0f };
    float playerRadius = 20.0f;
    float playerHp = 100.0f;
    int playerExp = 0;
    int defeatedCount = 0;
    int currentStage = 1;

    TestProjectile projectiles[20] = {};
    float spawnTimer = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        //  E키: 스테이지 증가 (새 몬스터 HP +5 확인용)
        if (IsKeyPressed(KEY_E)) currentStage++;

        //  WASD키로 목표(플레이어) 이동
        if (IsKeyDown(KEY_W)) targetPos.y -= 180.0f * dt;
        if (IsKeyDown(KEY_S)) targetPos.y += 180.0f * dt;
        if (IsKeyDown(KEY_A)) targetPos.x -= 180.0f * dt;
        if (IsKeyDown(KEY_D)) targetPos.x += 180.0f * dt;

        //  마우스 좌클릭으로 투사체 발사
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            float dx = mousePos.x - targetPos.x;
            float dy = mousePos.y - targetPos.y;
            float dist = sqrtf(dx * dx + dy * dy);
            if (dist > 0.0f) {
                for (int i = 0; i < 20; i++) {
                    if (!projectiles[i].active) {
                        projectiles[i] = { targetPos, { (dx / dist) * 450.0f, (dy / dist) * 450.0f }, 6.0f, 20.0f, true };
                        break;
                    }
                }
            }
        }

        //  1초마다 3종 몬스터 중 1마리 생성 (기존 SpawnSleepy 대신 SpawnRandomMonster 사용)
        spawnTimer += dt;
        if (spawnTimer >= SPAWN_INTERVAL) {
            SpawnRandomMonster(monsters, screenWidth, screenHeight, currentStage);
            spawnTimer = 0.0f;
        }

        UpdateMonsters(monsters, targetPos, dt);

        //  플레이어-몬스터 접촉 충돌 및 피해 판정
        float damageTaken = ResolvePlayerMonsterCombat(monsters, targetPos, playerRadius);
        if (damageTaken > 0.0f) {
            playerHp -= damageTaken;
            if (playerHp < 0.0f) playerHp = 0.0f;
        }

        //  투사체 이동 및 몬스터 피격/처치 판정
        for (int i = 0; i < 20; i++) {
            if (!projectiles[i].active) continue;

            projectiles[i].position.x += projectiles[i].velocity.x * dt;
            projectiles[i].position.y += projectiles[i].velocity.y * dt;

            if (projectiles[i].position.x < 0 || projectiles[i].position.x > screenWidth ||
                projectiles[i].position.y < 0 || projectiles[i].position.y > screenHeight) {
                projectiles[i].active = false;
                continue;
            }
            
            MonsterHitResult hitResult = ResolveProjectileMonsterCombat(
                monsters, projectiles[i].position, projectiles[i].radius, projectiles[i].damage
            );

            if (hitResult.hit) {
                projectiles[i].active = false;
                if (hitResult.killed) {
                    playerExp += hitResult.expReward;
                    defeatedCount++;
                }
            }
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);

        // 플레이어 원 표시
        DrawCircleV(targetPos, playerRadius, BLUE);

        // 투사체 그리기
        for (int i = 0; i < 20; i++) {
            if (projectiles[i].active) DrawCircleV(projectiles[i].position, projectiles[i].radius, YELLOW);
        }

        DrawMonsters(monsters);

        DrawText(TextFormat("Active Monsters: %d / %d", GetActiveMonsterCount(monsters), MAX_MONSTERS), 20, 20, 20, WHITE);
        DrawText(TextFormat("Stage: %d (+%d HP) | Player HP: %.0f | EXP: %d | Killed: %d", currentStage, (currentStage - 1) * STAGE_HP_INCREMENT, playerHp, playerExp, defeatedCount), 20, 50, 18, YELLOW);
        DrawText("WASD: Move | Left Click: Shoot | E: Next Stage", 20, screenHeight - 30, 16, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}