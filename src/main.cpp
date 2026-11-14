
#include "raylib.h"

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    // 게임 창 생성
    InitWindow(screenWidth, screenHeight, "Exam Season Survivors");

    // FPS 설정
    SetTargetFPS(60);

    // 게임 실행
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText(
            "Exam Season Survivors",
            400, 300, 40, DARKBLUE
        );

        DrawText(
            "raylib setup successful!",
            450, 370, 20, DARKGRAY
        );

        EndDrawing();
    }

    // 게임 창 종료
    CloseWindow();

    return 0;
}
