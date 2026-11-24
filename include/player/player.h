#pragma once

#include "raylib.h"

// 플레이어의 기본 정보
struct Player
{
    Vector2 position;  // 플레이어 위치
    float radius;      // 플레이어 반지름
    float speed;       // 플레이어 이동 속도

    // 체력 정보
    float hp = 100.0f;
    float maxHp = 100.0f;

    // 생존 상태
    bool isAlive = true;

    // 피격 무적 시간
    float invulnerabilityTimer = 0.0f;

    // 레벨 및 경험치
    int level = 1;
    int experience = 0;
    int requiredExperience = 50;
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

// 플레이어 피해 및 사망 처리
void DamagePlayer(Player& player, float damage);

// 플레이어 경험치 획득 및 레벨업 처리
bool GainExperience(Player& player, int amount);