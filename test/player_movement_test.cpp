#include "raylib.h"
#include "player/player.h"

int main()
{
    // 화면 크기 설정
    const int screenWidth = 1280;
    const int screenHeight = 720;

    // 게임 창 생성
    InitWindow(
        screenWidth,
        screenHeight,
        "Player Movement Test"
    );

    SetTargetFPS(60);

    // 플레이어 생성 및 초기 위치 지정
    Player player = CreatePlayer({
        screenWidth / 2.0f,
        screenHeight / 2.0f
    });

    // 게임 반복문
   while (!WindowShouldClose())
{
    // 프레임 간 시간 계산
    float deltaTime = GetFrameTime();

    // 플레이어 이동 업데이트
    UpdatePlayer(
        player,
        deltaTime,
        screenWidth,
        screenHeight
    );

    BeginDrawing();

    ClearBackground(BLACK);

    // 플레이어 그리기
    DrawPlayer(player);

    EndDrawing();
}

    // 게임 창 종료
    CloseWindow();

    return 0;
}