#ifndef MONSTER_H
#define MONSTER_H

#include "raylib.h"

#define MAX_MONSTERS 25
#define SPAWN_INTERVAL 1.0f
#define SPAWN_PADDING 50.0f
#define STAGE_HP_INCREMENT 5    // 스테이지당 몬스터 HP 증가량

// [추가] 몬스터 3종 구분
enum MonsterType {
    MONSTER_SLEEPY,       // 졸음이
    MONSTER_POP_QUIZ,     // 쪽지시험
    MONSTER_TEAM_PROJECT  // 팀프로젝트
};

struct Monster {
    Vector2 position;
    float hp;
    float maxHp;
    float speed;
    float attack;
    int expReward;
    float spriteRadius;
    float collisionRadius;
    bool active;

    // 몬스터 종류 및 접촉 공격 쿨타임
    MonsterType type;
    float attackCooldown;
};

// 몬스터 피격 및 사망 결과
struct MonsterHitResult {
    bool hit;
    bool killed;
    int expReward;
    Vector2 position;
};

void InitMonsters(Monster monsters[]);

void SpawnSleepy(Monster monsters[], int screenWidth, int screenHeight, int currentStage = 1);

// 쪽지시험, 팀프로젝트 및 3종 랜덤 생성 함수
void SpawnPopQuiz(Monster monsters[], int screenWidth, int screenHeight, int currentStage = 1);
void SpawnTeamProject(Monster monsters[], int screenWidth, int screenHeight, int currentStage = 1);
void SpawnRandomMonster(Monster monsters[], int screenWidth, int screenHeight, int currentStage = 1);

void UpdateMonsters(Monster monsters[], Vector2 targetPos, float deltaTime);

// 몬스터 HP 감소 및 사망 처리 함수
MonsterHitResult DamageMonster(Monster& monster, float damage);

void DrawMonsters(const Monster monsters[]);
int GetActiveMonsterCount(const Monster monsters[]);

#endif