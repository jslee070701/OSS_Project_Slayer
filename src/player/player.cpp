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

// 플레이어 이동
void UpdatePlayer(
    Player& player,
    float deltaTime,
    int screenWidth,
    int screenHeight
)
{
    // 화면 크기는 다음 단계의 경계 제한에 사용 예정
    (void)screenWidth;
    (void)screenHeight;

    // W: 위쪽 이동
    if (IsKeyDown(KEY_W))
    {
        player.position.y -= player.speed * deltaTime;
    }

    // S: 아래쪽 이동
    if (IsKeyDown(KEY_S))
    {
        player.position.y += player.speed * deltaTime;
    }

    // A: 왼쪽 이동
    if (IsKeyDown(KEY_A))
    {
        player.position.x -= player.speed * deltaTime;
    }

    // D: 오른쪽 이동
    if (IsKeyDown(KEY_D))
    {
        player.position.x += player.speed * deltaTime;
    }
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