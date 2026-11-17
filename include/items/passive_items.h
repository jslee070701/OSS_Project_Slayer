#pragma once
#include <array>
#include "raylib.h"

namespace passive_items
{
        // 동시에 존재할 수 있는 학식 개수
    constexpr int MAX_MEAL_DROPS = 2;

    // 임시 충돌 반지름: 추후 아이템 이미지에 맞게 조정
    constexpr float MEAL_RADIUS = 12.0f;

    struct MealDrop
    {
        Vector2 position = { 0.0f, 0.0f };
        bool active = false;
    };

    struct MealDropSystem
    {
        std::array<MealDrop, MAX_MEAL_DROPS> drops{};
    };

    // 테스트 가능한 드롭 판정: roll은 1~100
    bool TryDropMeal(
        MealDropSystem& system,
        Vector2 enemyPosition,
        int roll
    );

    // 몬스터 사망 시 호출하는 실제 무작위 드롭 함수
    bool OnEnemyDefeated(
        MealDropSystem& system,
        Vector2 enemyPosition
    );

    // 충돌한 학식 회수 및 회복 처리, 획득 개수 반환
    int CollectMeals(
        MealDropSystem& system,
        Vector2 playerPosition,
        float playerCollisionRadius,
        float& hp
    );
    // 패시브 아이템 종류
    enum class ItemType
    {
        Coffee,
        Highlighter,
        PastExam
    };

    // 아이템별 최대 보유 개수
    constexpr int MAX_ITEM_COUNT = 5;

    // 보유 중인 패시브 아이템 개수
    struct Inventory
    {
        int coffeeCount = 0;
        int highlighterCount = 0;
        int pastExamCount = 0;
    };

    // 아이템 획득: 획득 성공 시 true, 최대 개수이면 false 반환
    bool TryAcquireItem(Inventory& inventory, ItemType item);

    // 보유 개수에 따른 배율 계산
    float GetSpeedMultiplier(const Inventory& inventory);
    float GetDamageMultiplier(const Inventory& inventory);
    float GetExperienceMultiplier(const Inventory& inventory);

    // 기본 수치에 아이템 효과를 적용한 최종 수치 계산
    float CalculateSpeed(float baseSpeed, const Inventory& inventory);
    float CalculateDamage(float baseDamage, const Inventory& inventory);
    float CalculateExperience(float baseExperience, const Inventory& inventory);

    // 학식 획득: HP를 20 회복하며 최대 100으로 제한
    void ApplyMealHealing(float& hp);
}