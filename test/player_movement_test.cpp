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
        BeginDrawing();

        // 검은색 배경
        ClearBackground(BLACK);

        // 플레이어 그리기
        DrawPlayer(player);

        EndDrawing();
    }

    // 게임 창 종료
    CloseWindow();

    return 0;
}