#include "monsters/monster.h"
#include <cmath>

// 화면 외곽 4방향 + 50px 여백 스폰 좌표 설정 (기존 SpawnSleepy의 좌표 로직 분리)
static void SetRandomSpawnPosition(Monster& monster, int screenWidth, int screenHeight) {
    int side = GetRandomValue(0, 3);
    switch (side) {
        case 0: // 위쪽 바깥
            monster.position.x = (float)GetRandomValue(0, screenWidth);
            monster.position.y = (float)GetRandomValue(-(int)SPAWN_PADDING, 0);
            break;
        case 1: // 아래쪽 바깥
            monster.position.x = (float)GetRandomValue(0, screenWidth);
            monster.position.y = (float)GetRandomValue(screenHeight, screenHeight + (int)SPAWN_PADDING);
            break;
        case 2: // 왼쪽 바깥
            monster.position.x = (float)GetRandomValue(-(int)SPAWN_PADDING, 0);
            monster.position.y = (float)GetRandomValue(0, screenHeight);
            break;
        case 3: // 오른쪽 바깥
            monster.position.x = (float)GetRandomValue(screenWidth, screenWidth + (int)SPAWN_PADDING);
            monster.position.y = (float)GetRandomValue(0, screenHeight);
            break;
    }
}

void InitMonsters(Monster monsters[]) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        monsters[i].active = false;
        monsters[i].attackCooldown = 0.0f; // [추가]
    }
}

// 1. 졸음이 생성 (스테이지 HP 보너스 추가)
void SpawnSleepy(Monster monsters[], int screenWidth, int screenHeight, int currentStage) {
    if (GetActiveMonsterCount(monsters) >= MAX_MONSTERS) return;

    float bonusHp = (currentStage > 1) ? (float)((currentStage - 1) * STAGE_HP_INCREMENT) : 0.0f;

    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) {
            monsters[i].active = true;
            monsters[i].type = MONSTER_SLEEPY;
            monsters[i].maxHp = 40.0f + bonusHp;
            monsters[i].hp = monsters[i].maxHp;
            monsters[i].speed = 10.0f;
            monsters[i].attack = 10.0f;
            monsters[i].expReward = 10;
            monsters[i].spriteRadius = 20.0f;
            monsters[i].collisionRadius = monsters[i].spriteRadius * 0.8f;
            monsters[i].attackCooldown = 0.0f;

            SetRandomSpawnPosition(monsters[i], screenWidth, screenHeight);
            break;
        }
    }
}

// 2. 쪽지시험 생성 (빠르고 약한 돌격형)
void SpawnPopQuiz(Monster monsters[], int screenWidth, int screenHeight, int currentStage) {
    if (GetActiveMonsterCount(monsters) >= MAX_MONSTERS) return;

    float bonusHp = (currentStage > 1) ? (float)((currentStage - 1) * STAGE_HP_INCREMENT) : 0.0f;

    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) {
            monsters[i].active = true;
            monsters[i].type = MONSTER_POP_QUIZ;
            monsters[i].maxHp = 25.0f + bonusHp;
            monsters[i].hp = monsters[i].maxHp;
            monsters[i].speed = 25.0f;
            monsters[i].attack = 15.0f;
            monsters[i].expReward = 15;
            monsters[i].spriteRadius = 16.0f;
            monsters[i].collisionRadius = monsters[i].spriteRadius * 0.8f;
            monsters[i].attackCooldown = 0.0f;

            SetRandomSpawnPosition(monsters[i], screenWidth, screenHeight);
            break;
        }
    }
}

// 3. 팀프로젝트 생성 (느리지만 체력과 공격력이 높은 탱커형)
void SpawnTeamProject(Monster monsters[], int screenWidth, int screenHeight, int currentStage) {
    if (GetActiveMonsterCount(monsters) >= MAX_MONSTERS) return;

    float bonusHp = (currentStage > 1) ? (float)((currentStage - 1) * STAGE_HP_INCREMENT) : 0.0f;

    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) {
            monsters[i].active = true;
            monsters[i].type = MONSTER_TEAM_PROJECT;
            monsters[i].maxHp = 100.0f + bonusHp;
            monsters[i].hp = monsters[i].maxHp;
            monsters[i].speed = 7.0f;
            monsters[i].attack = 25.0f;
            monsters[i].expReward = 30;
            monsters[i].spriteRadius = 28.0f;
            monsters[i].collisionRadius = monsters[i].spriteRadius * 0.8f;
            monsters[i].attackCooldown = 0.0f;

            SetRandomSpawnPosition(monsters[i], screenWidth, screenHeight);
            break;
        }
    }
}

//  3종 중 확률적으로 1마리 생성 (졸음이 50%, 쪽지시험 30%, 팀프로젝트 20%)
void SpawnRandomMonster(Monster monsters[], int screenWidth, int screenHeight, int currentStage) {
    int roll = GetRandomValue(1, 100);
    if (roll <= 50) {
        SpawnSleepy(monsters, screenWidth, screenHeight, currentStage);
    } else if (roll <= 80) {
        SpawnPopQuiz(monsters, screenWidth, screenHeight, currentStage);
    } else {
        SpawnTeamProject(monsters, screenWidth, screenHeight, currentStage);
    }
}

void UpdateMonsters(Monster monsters[], Vector2 targetPos, float deltaTime) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) continue;

        // 접촉 공격 쿨타임 감소
        if (monsters[i].attackCooldown > 0.0f) {
            monsters[i].attackCooldown -= deltaTime;
        }

        // 기존 플레이어 추격 이동 로직 그대로 유지
        float dx = targetPos.x - monsters[i].position.x;
        float dy = targetPos.y - monsters[i].position.y;
        float distance = sqrtf(dx * dx + dy * dy);

        if (distance > 1.0f) {
            monsters[i].position.x += (dx / distance) * monsters[i].speed * deltaTime;
            monsters[i].position.y += (dy / distance) * monsters[i].speed * deltaTime;
        }
    }
}

// [추가] 몬스터 피격 및 사망 처리
MonsterHitResult DamageMonster(Monster& monster, float damage) {
    MonsterHitResult result = { false, false, 0, { 0.0f, 0.0f } };
    if (!monster.active || damage <= 0.0f) return result;

    result.hit = true;
    result.position = monster.position;
    monster.hp -= damage;

    if (monster.hp <= 0.0f) {
        monster.hp = 0.0f;
        monster.active = false;
        result.killed = true;
        result.expReward = monster.expReward;
    }

    return result;
}

void DrawMonsters(const Monster monsters[]) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) continue;

        // [추가] 몬스터 종류별 색상 구분
        Color bodyColor = PURPLE; // 졸음이
        if (monsters[i].type == MONSTER_POP_QUIZ) bodyColor = ORANGE;
        else if (monsters[i].type == MONSTER_TEAM_PROJECT) bodyColor = MAROON;

        DrawCircleV(monsters[i].position, monsters[i].spriteRadius, bodyColor);
        DrawCircleLines((int)monsters[i].position.x, (int)monsters[i].position.y, monsters[i].collisionRadius, GREEN);
    }
}

int GetActiveMonsterCount(const Monster monsters[]) {
    int count = 0;
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (monsters[i].active) count++;
    }
    return count;
}