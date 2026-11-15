#pragma once

#include "raylib.h"

// 플레이어의 기본 정보
struct Player
{
    Vector2 position;  // 플레이어 위치
    float radius;      // 플레이어 반지름
    float speed;       // 플레이어 이동 속도
};

// 플레이어 생성
Player CreatePlayer(Vector2 startPosition);

// 플레이어 이동
void UpdatePlayer(
    Player& player,
    float deltaTime,
    int screenWidth,
    int screenHeight
);

// 플레이어 그리기
void DrawPlayer(const Player& player);