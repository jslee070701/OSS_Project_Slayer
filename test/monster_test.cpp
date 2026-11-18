#include "raylib.h"
#include "monsters/monster.h"

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Exam Season Survivors - Monster Test");
    SetTargetFPS(60);

    Monster monsters[MAX_MONSTERS];
    InitMonsters(monsters);

    // 몬스터가 향해 움직일 임의의 목표 좌표
    Vector2 targetPos = { 640.0f, 360.0f };
    float spawnTimer = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // 1초마다 졸음이 1마리씩 생성 (최대 25마리)
        spawnTimer += dt;
        if (spawnTimer >= SPAWN_INTERVAL) {
            SpawnSleepy(monsters, screenWidth, screenHeight);
            spawnTimer = 0.0f;
        }

        // 지정된 임의 좌표를 향해 몬스터 이동
        UpdateMonsters(monsters, targetPos, dt);

        BeginDrawing();
        ClearBackground(DARKGRAY);

        // 목표 좌표 위치 표시 (십자선 및 좌표 텍스트)
        DrawCircleV(targetPos, 6.0f, RED);
        DrawLine((int)targetPos.x - 15, (int)targetPos.y, (int)targetPos.x + 15, (int)targetPos.y, RED);
        DrawLine((int)targetPos.x, (int)targetPos.y - 15, (int)targetPos.x, (int)targetPos.y + 15, RED);
        DrawText(TextFormat("Target (%.0f, %.0f)", targetPos.x, targetPos.y), (int)targetPos.x + 10, (int)targetPos.y - 20, 16, RED);

        // 졸음이 몬스터 그리기
        DrawMonsters(monsters);

        // 생성 수 확인용 텍스트
        DrawText(TextFormat("Active Monsters: %d / %d", GetActiveMonsterCount(monsters), MAX_MONSTERS), 20, 20, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}