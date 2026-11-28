#ifndef MONSTER_H
#define MONSTER_H

#include "raylib.h"
#include <stdbool.h>

// 일반 몬스터 상수
#define MAX_MONSTERS 25
#define SPAWN_INTERVAL 1.0f
#define SPAWN_PADDING 50.0f
#define STAGE_HP_INCREMENT 5
#define MONSTER_ATTACK_COOLDOWN 1.0f

// 졸음이 기준 크기 
#define SLEEPY_RADIUS 15.0f

// 일반 몬스터 종류
typedef enum {
    MONSTER_SLEEPY = 0,     // 졸음이
    MONSTER_POP_QUIZ,       // 쪽지시험
    MONSTER_TEAM_PROJECT    // 팀프로젝트
} MonsterType;

// 보스 종류
typedef enum {
    BOSS_MIDTERM = 0,       // 중간고사 보스
    BOSS_FINAL              // 기말고사 보스
} BossType;

// 일반 몬스터 구조체
typedef struct {
    bool active;
    MonsterType type;
    Vector2 position;
    float hp;
    float maxHp;
    float speed;
    float attack;
    int expReward;
    float collisionRadius;
    float attackCooldown;
} Monster;

// 보스 구조체
typedef struct {
    bool active;
    BossType type;
    Vector2 position;
    float hp;
    float maxHp;
    float speed;
    float attack;
    int expReward;
    float collisionRadius;
    float attackCooldown;
} Boss;

// 공격이나 피격 결과 구조체 (App 전달용)
typedef struct {
    bool hit;                   // 명중 여부
    bool killed;                // 처치 여부
    int expReward;              // 획득 경험치
    Vector2 position;           // 피격/사망 좌표 (아이템 드롭 위치용)
    bool isBoss;                // 보스 여부 (true: 보스, false: 일반 몬스터)
    MonsterType monsterType;    // 일반 몬스터일 경우 종류 (졸음이/쪽지시험/팀플)
    BossType bossType;          // 보스일 경우 보스 종류 (중간고사/기말고사)
} MonsterHitResult;

// 일반 몬스터 함수
void InitMonsters(Monster monsters[]);
void SpawnSleepy(Monster monsters[], int currentStage);
void SpawnPopQuiz(Monster monsters[], int currentStage);
void SpawnTeamProject(Monster monsters[], int currentStage);
void SpawnRandomMonster(Monster monsters[], int currentStage);
void UpdateMonsters(Monster monsters[], Vector2 targetPos, float deltaTime);
void DrawMonsters(const Monster monsters[]);
MonsterHitResult DamageMonster(Monster* monster, float damage);

// 보스 몬스터 함수
void InitBoss(Boss* boss);
void SpawnMidtermBoss(Boss* boss);
void SpawnFinalBoss(Boss* boss);
void UpdateBoss(Boss* boss, Vector2 targetPos, float deltaTime);
void DrawBoss(const Boss* boss);
void DrawBossHpBar(const Boss* boss, int screenWidth);
MonsterHitResult DamageBoss(Boss* boss, float damage);

// 탐색 함수 (App 자동 조준/무기 발사용)
// 활성화된 일반 몬스터 및 보스 중 origin에서 가장 가까운 대상의 좌표를 outTargetPos에 저장
// 대상이 하나라도 있으면 true, 없으면 false 반환
bool FindNearestMonsterPosition(const Monster monsters[], const Boss* boss, Vector2 origin, Vector2* outTargetPos);

#endif // MONSTER_H