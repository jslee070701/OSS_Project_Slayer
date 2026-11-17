//monster header
#ifndef MONSTER_H
#define MONSTER_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_MONSTERS 25         // 동시에 최대 스폰 수 25마리
#define SPAWN_INTERVAL 1.0f     // 1초에 1마리 스폰
#define SPAWN_PADDING 50.0f     // 화면 외곽 패딩 50px
#define SPEED_SCALE 15.0f       // 속도(조정 가능)

typedef struct {
    Vector2 position;           // 현재 중심 좌표
    float speed;                // 이동 속도
    float spriteRadius;         // 기본 반지름
    float collisionRadius;      // 충돌 범위 반지름 
    int hp;                     // 체력
    int attackPower;            // 공격력
    int expReward;              // 처치 시 지급 경험치
    bool active;                // 생존 상태 여부
} Monster;

void InitMonsters(Monster monsters[]);
int GetActiveMonsterCount(const Monster monsters[]);
void SpawnSleepy(Monster monsters[], int screenWidth, int screenHeight);
void UpdateMonsters(Monster monsters[], Vector2 targetPos, float deltaTime);
void DrawMonsters(const Monster monsters[]);

#endif