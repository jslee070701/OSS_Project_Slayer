
#include "app/app.h"

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    // 게임 창 생성
    InitWindow(screenWidth, screenHeight, "Exam Season Survivors");

    // FPS 설정
    SetTargetFPS(60);

    // App 객체 생성 및 초기화
    App app{};
    InitApp(app, screenWidth, screenHeight);

    // 게임 실행
    while (!WindowShouldClose() && !app.shouldExit)
    {
        // 프레임 간 시간 간격 계산
        float deltaTime = GetFrameTime();

        // 게임 상태 업데이트
        UpdateApp(
            app,
            deltaTime,
            screenWidth,
            screenHeight
        );

        // 화면 출력
        BeginDrawing();

        DrawApp(app);

        EndDrawing();
    }

    // App 종료 및 정리
    ShutdownApp(app);

    // 게임 창 종료
    CloseWindow();

    return 0;
}
