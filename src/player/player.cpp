#include "player/player.h"
#include <cmath>

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
// 플레이어 이동
void UpdatePlayer(
    Player& player,
    float deltaTime,
    int screenWidth,
    int screenHeight
)
{
    // 이동 방향
    Vector2 direction = { 0.0f, 0.0f };

    // 키보드 입력
    if (IsKeyDown(KEY_W)) direction.y -= 1.0f;
    if (IsKeyDown(KEY_S)) direction.y += 1.0f;
    if (IsKeyDown(KEY_A)) direction.x -= 1.0f;
    if (IsKeyDown(KEY_D)) direction.x += 1.0f;

    // 이동 방향의 길이 계산
    float length = std::sqrt(
        direction.x * direction.x +
        direction.y * direction.y
    );

    // 대각선 이동 속도 보정
    if (length > 0.0f)
    {
        direction.x /= length;
        direction.y /= length;
    }

    // 플레이어 위치 갱신
    player.position.x += direction.x * player.speed * deltaTime;
    player.position.y += direction.y * player.speed * deltaTime;

    // 화면 경계 제한
    if (player.position.x < player.radius)
        player.position.x = player.radius;

    if (player.position.x > screenWidth - player.radius)
        player.position.x = screenWidth - player.radius;

    if (player.position.y < player.radius)
        player.position.y = player.radius;

    if (player.position.y > screenHeight - player.radius)
        player.position.y = screenHeight - player.radius;
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