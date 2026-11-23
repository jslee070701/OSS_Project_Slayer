
#include "raylib.h"
#include "app/app.h"

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "App Integration Test");
    SetTargetFPS(60);

    App app;

    // App 초기화
    InitApp(app, screenWidth, screenHeight);

    // 메인 테스트 루프
    while (!WindowShouldClose() && !app.shouldExit)
    {
        float deltaTime = GetFrameTime();

        // App 업데이트
        UpdateApp(app, deltaTime, screenWidth, screenHeight);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // App 화면 출력
        DrawApp(app);

        EndDrawing();
    }

    // App 정리
    ShutdownApp(app);

    CloseWindow();

    return 0;
}