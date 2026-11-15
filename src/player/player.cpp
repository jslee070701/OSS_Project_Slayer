#include "player/player.h"

// 플레이어 생성
Player CreatePlayer(Vector2 startPosition)
{
    Player player{};

    // 초기 위치 설정
    player.position = startPosition;

    // 플레이어 크기 설정
    player.radius = 20.0f;

    // 플레이어 이동 속도 설정
    player.speed = 250.0f;

    return player;
}

// 플레이어 그리기
void DrawPlayer(const Player& player)
{
    DrawCircleV(
        player.position,
        player.radius,
        BLUE
    );
}