#include "monsters/monster.h"
#include <math.h>

// 몬스터 배열 초기화
void InitMonsters(Monster monsters[]) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        monsters[i].active = false;
    }
}

// 현재 활성화된 몬스터 수 반환
int GetActiveMonsterCount(const Monster monsters[]) {
    int count = 0;
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (monsters[i].active) {
            count++;
        }
    }
    return count;
}

// 졸음이(일반몬스터) 생성
void SpawnSleepy(Monster monsters[], int screenWidth, int screenHeight) {
    // 최대 25마리 제한 체크
    if (GetActiveMonsterCount(monsters) >= MAX_MONSTERS) {
        return;
    }

    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) {
            monsters[i].active = true;
            monsters[i].hp = 40;
            monsters[i].speed = 10.0f * SPEED_SCALE;
            monsters[i].attackPower = 10;
            monsters[i].expReward = 10;

            // 스프라이트 크기 및 80% 원형 충돌 범위 설정
            monsters[i].spriteRadius = 20.0f;
            monsters[i].collisionRadius = monsters[i].spriteRadius * 0.8f;

            // 화면 바깥쪽 4면 외곽(패딩 50px 범위) 무작위 좌표 계산
            int side = GetRandomValue(0, 3); // 0: 상, 1: 하, 2: 좌, 3: 우
            float pad = SPAWN_PADDING;

            if (side == 0) { // 위쪽 외곽
                monsters[i].position.x = (float)GetRandomValue((int)(-pad), (int)(screenWidth + pad));
                monsters[i].position.y = (float)GetRandomValue((int)(-pad), 0);
            } else if (side == 1) { // 아래쪽 외곽
                monsters[i].position.x = (float)GetRandomValue((int)(-pad), (int)(screenWidth + pad));
                monsters[i].position.y = (float)GetRandomValue(screenHeight, (int)(screenHeight + pad));
            } else if (side == 2) { // 왼쪽 외곽
                monsters[i].position.x = (float)GetRandomValue((int)(-pad), 0);
                monsters[i].position.y = (float)GetRandomValue((int)(-pad), (int)(screenHeight + pad));
            } else { // 오른쪽 외곽
                monsters[i].position.x = (float)GetRandomValue(screenWidth, (int)(screenWidth + pad));
                monsters[i].position.y = (float)GetRandomValue((int)(-pad), (int)(screenHeight + pad));
            }
            break;
        }
    }
}

// 임시 플레이어 좌표를 향한 추격 로직
void UpdateMonsters(Monster monsters[], Vector2 targetPos, float deltaTime) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) {
            continue;
        }

        float dx = targetPos.x - monsters[i].position.x;
        float dy = targetPos.y - monsters[i].position.y;
        float distance = sqrtf(dx * dx + dy * dy);

        // 목표 좌표와 겹치기 전까지 목표 방향으로 이동
        if (distance > 1.0f) {
            monsters[i].position.x += (dx / distance) * monsters[i].speed * deltaTime;
            monsters[i].position.y += (dy / distance) * monsters[i].speed * deltaTime;
        }
    }
}

// 몬스터 그리기 (도형)
void DrawMonsters(const Monster monsters[]) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) {
            continue;
        }

        // 스프라이트 영역
        DrawCircleV(monsters[i].position, monsters[i].spriteRadius, PURPLE);
        // 충돌 범위 확인용 초록색 테두리
        DrawCircleLines((int)monsters[i].position.x, (int)monsters[i].position.y, monsters[i].collisionRadius, GREEN);
        DrawText("Z", (int)monsters[i].position.x - 4, (int)monsters[i].position.y - 8, 16, YELLOW);
    }
}