#include "monsters/monster.h"
#include <math.h>
#include <float.h>

// 화면 밖 4방향 무작위 스폰 좌표 생성
static Vector2 GetRandomSpawnPosition(void) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int side = GetRandomValue(0, 3);
    Vector2 pos = { 0.0f, 0.0f };

    switch (side) {
        case 0: // 상단
            pos.x = (float)GetRandomValue(0, screenWidth);
            pos.y = -SPAWN_PADDING;
            break;
        case 1: // 하단
            pos.x = (float)GetRandomValue(0, screenWidth);
            pos.y = (float)screenHeight + SPAWN_PADDING;
            break;
        case 2: // 좌측
            pos.x = -SPAWN_PADDING;
            pos.y = (float)GetRandomValue(0, screenHeight);
            break;
        case 3: // 우측
            pos.x = (float)screenWidth + SPAWN_PADDING;
            pos.y = (float)GetRandomValue(0, screenHeight);
            break;
    }
    return pos;
}

static int FindInactiveMonsterSlot(const Monster monsters[]) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) return i;
    }
    return -1;
}

void InitMonsters(Monster monsters[]) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        monsters[i].active = false;
    }
}

// 졸음이
void SpawnSleepy(Monster monsters[], int currentStage) {
    int idx = FindInactiveMonsterSlot(monsters);
    if (idx == -1) return;

    float bonusHp = (currentStage > 1) ? (float)((currentStage - 1) * STAGE_HP_INCREMENT) : 0.0f;

    monsters[idx].active = true;
    monsters[idx].type = MONSTER_SLEEPY;
    monsters[idx].position = GetRandomSpawnPosition();
    monsters[idx].maxHp = 40.0f + bonusHp;
    monsters[idx].hp = monsters[idx].maxHp;
    monsters[idx].speed = 10.0f;
    monsters[idx].attack = 10.0f;
    monsters[idx].expReward = 10;
    monsters[idx].collisionRadius = SLEEPY_RADIUS;
    monsters[idx].attackCooldown = 0.0f;
}

// 쪽지시험
void SpawnPopQuiz(Monster monsters[], int currentStage) {
    int idx = FindInactiveMonsterSlot(monsters);
    if (idx == -1) return;

    float bonusHp = (currentStage > 1) ? (float)((currentStage - 1) * STAGE_HP_INCREMENT) : 0.0f;

    monsters[idx].active = true;
    monsters[idx].type = MONSTER_POP_QUIZ;
    monsters[idx].position = GetRandomSpawnPosition();
    monsters[idx].maxHp = 30.0f + bonusHp;
    monsters[idx].hp = monsters[idx].maxHp;
    monsters[idx].speed = 20.0f;
    monsters[idx].attack = 10.0f;
    monsters[idx].expReward = 15;
    monsters[idx].collisionRadius = 13.0f;
    monsters[idx].attackCooldown = 0.0f;
}

// 팀프로젝트
void SpawnTeamProject(Monster monsters[], int currentStage) {
    int idx = FindInactiveMonsterSlot(monsters);
    if (idx == -1) return;

    float bonusHp = (currentStage > 1) ? (float)((currentStage - 1) * STAGE_HP_INCREMENT) : 0.0f;

    monsters[idx].active = true;
    monsters[idx].type = MONSTER_TEAM_PROJECT;
    monsters[idx].position = GetRandomSpawnPosition();
    monsters[idx].maxHp = 200.0f + bonusHp;
    monsters[idx].hp = monsters[idx].maxHp;
    monsters[idx].speed = 5.0f;
    monsters[idx].attack = 20.0f;
    monsters[idx].expReward = 40;
    monsters[idx].collisionRadius = 22.0f;
    monsters[idx].attackCooldown = 0.0f;
}

void SpawnRandomMonster(Monster monsters[], int currentStage) {
    int roll = GetRandomValue(1, 100);
    if (roll <= 50) {
        SpawnSleepy(monsters, currentStage);
    } else if (roll <= 80) {
        SpawnPopQuiz(monsters, currentStage);
    } else {
        SpawnTeamProject(monsters, currentStage);
    }
}

void UpdateMonsters(Monster monsters[], Vector2 targetPos, float deltaTime) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) continue;

        if (monsters[i].attackCooldown > 0.0f) {
            monsters[i].attackCooldown -= deltaTime;
        }

        float dx = targetPos.x - monsters[i].position.x;
        float dy = targetPos.y - monsters[i].position.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist > 0.001f) {
            monsters[i].position.x += (dx / dist) * monsters[i].speed * deltaTime;
            monsters[i].position.y += (dy / dist) * monsters[i].speed * deltaTime;
        }
    }
}

void DrawMonsters(const Monster monsters[]) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) continue;

        Color color = GRAY;
        if (monsters[i].type == MONSTER_SLEEPY) color = SKYBLUE;
        else if (monsters[i].type == MONSTER_POP_QUIZ) color = ORANGE;
        else if (monsters[i].type == MONSTER_TEAM_PROJECT) color = PURPLE;

        DrawCircleV(monsters[i].position, monsters[i].collisionRadius, color);
    }
}

MonsterHitResult DamageMonster(Monster* monster, float damage) {
    MonsterHitResult result = { false, false, 0, { 0.0f, 0.0f }, false, BOSS_MIDTERM };
    if (monster == NULL || !monster->active) return result;

    result.hit = true;
    result.position = monster->position;
    monster->hp -= damage;

    if (monster->hp <= 0.0f) {
        monster->hp = 0.0f;
        monster->active = false;
        result.killed = true;
        result.expReward = monster->expReward;
    }

    return result;
}

// 보스 구현

void InitBoss(Boss* boss) {
    if (boss == NULL) return;
    boss->active = false;
}
// 보스 중간고사
void SpawnMidtermBoss(Boss* boss) {
    if (boss == NULL) return;

    boss->active = true;
    boss->type = BOSS_MIDTERM;
    boss->position = GetRandomSpawnPosition();
    boss->maxHp = 1000.0f;
    boss->hp = boss->maxHp;
    boss->speed = 5.0f;
    boss->attack = 50.0f;
    boss->expReward = 300;
    boss->collisionRadius = SLEEPY_RADIUS * 2.0f; // 30.0f
    boss->attackCooldown = 0.0f;
}
// 보스 기말고사
void SpawnFinalBoss(Boss* boss) {
    if (boss == NULL) return;

    boss->active = true;
    boss->type = BOSS_FINAL;
    boss->position = GetRandomSpawnPosition();
    boss->maxHp = 2000.0f;
    boss->hp = boss->maxHp;
    boss->speed = 5.0f;
    boss->attack = 50.0f;
    boss->expReward = 1000;
    boss->collisionRadius = SLEEPY_RADIUS * 3.0f; // 45.0f
    boss->attackCooldown = 0.0f;
}

void UpdateBoss(Boss* boss, Vector2 targetPos, float deltaTime) {
    if (boss == NULL || !boss->active) return;

    if (boss->attackCooldown > 0.0f) {
        boss->attackCooldown -= deltaTime;
    }

    float dx = targetPos.x - boss->position.x;
    float dy = targetPos.y - boss->position.y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist > 0.001f) {
        boss->position.x += (dx / dist) * boss->speed * deltaTime;
        boss->position.y += (dy / dist) * boss->speed * deltaTime;
    }
}

void DrawBoss(const Boss* boss) {
    if (boss == NULL || !boss->active) return;

    Color bodyColor = (boss->type == BOSS_MIDTERM) ? MAROON : DARKPURPLE;
    DrawCircleV(boss->position, boss->collisionRadius, bodyColor);
    DrawCircleLines((int)boss->position.x, (int)boss->position.y, boss->collisionRadius + 3.0f, GOLD);
}

// 화면 상단 중앙 보스 HP 바 렌더링
void DrawBossHpBar(const Boss* boss, int screenWidth) {
    if (boss == NULL || !boss->active) return;

    int barWidth = 460;
    int barHeight = 22;
    int barX = (screenWidth - barWidth) / 2;
    int barY = 55; // 상단 HUD 아래 여백

    float ratio = (boss->maxHp > 0.0f) ? (boss->hp / boss->maxHp) : 0.0f;
    if (ratio < 0.0f) ratio = 0.0f;

    const char* title = (boss->type == BOSS_MIDTERM) ? "BOSS: MIDTERM EXAM" : "BOSS: FINAL EXAM";

    DrawText(title, barX, barY - 22, 18, RED);
    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    DrawRectangle(barX, barY, (int)(barWidth * ratio), barHeight, RED);
    DrawRectangleLines(barX, barY, barWidth, barHeight, WHITE);

    DrawText(TextFormat("%.0f / %.0f", boss->hp, boss->maxHp), barX + barWidth / 2 - 45, barY + 3, 16, WHITE);
}

MonsterHitResult DamageBoss(Boss* boss, float damage) {
    MonsterHitResult result = { false, false, 0, { 0.0f, 0.0f }, true, BOSS_MIDTERM };
    if (boss == NULL || !boss->active) return result;

    result.hit = true;
    result.position = boss->position;
    result.isBoss = true;
    result.bossType = boss->type;

    boss->hp -= damage;
    if (boss->hp <= 0.0f) {
        boss->hp = 0.0f;
        boss->active = false;
        result.killed = true;
        result.expReward = boss->expReward;
    }

    return result;
}

// 최단 거리 몬스터 탐색

bool FindNearestMonsterPosition(const Monster monsters[], const Boss* boss, Vector2 origin, Vector2* outTargetPos) {
    if (outTargetPos == NULL) return false;

    float minDistSq = FLT_MAX;
    bool found = false;

    // 일반 몬스터 25마리 탐색
    if (monsters != NULL) {
        for (int i = 0; i < MAX_MONSTERS; i++) {
            if (!monsters[i].active) continue;

            float dx = monsters[i].position.x - origin.x;
            float dy = monsters[i].position.y - origin.y;
            float distSq = dx * dx + dy * dy;

            if (distSq < minDistSq) {
                minDistSq = distSq;
                *outTargetPos = monsters[i].position;
                found = true;
            }
        }
    }

    // 보스도 활성화되어 있으면 거리 비교에 포함
    if (boss != NULL && boss->active) {
        float dx = boss->position.x - origin.x;
        float dy = boss->position.y - origin.y;
        float distSq = dx * dx + dy * dy;

        if (distSq < minDistSq) {
            minDistSq = distSq;
            *outTargetPos = boss->position;
            found = true;
        }
    }

    return found;
}