#include "items/passive_items.h"

#include <algorithm>

namespace passive_items
{
    bool TryAcquireItem(Inventory& inventory, ItemType item)
    {
        int* count = nullptr;

        // 획득한 아이템에 해당하는 보유 개수 선택
        switch (item)
        {
        case ItemType::Coffee:
            count = &inventory.coffeeCount;
            break;

        case ItemType::Highlighter:
            count = &inventory.highlighterCount;
            break;

        case ItemType::PastExam:
            count = &inventory.pastExamCount;
            break;

        default:
            return false;
        }

        // 최대 개수에 도달하면 추가 획득 불가
        if (*count >= MAX_ITEM_COUNT)
        {
            return false;
        }

        ++(*count);
        return true;
    }

    // 중첩 방식은 기본 수치 기준 합연산으로 가정
    // 예: 커피 5개 = 이동속도 15% 증가
    float GetSpeedMultiplier(const Inventory& inventory)
    {
        const int count = std::clamp(
            inventory.coffeeCount, 0, MAX_ITEM_COUNT
        );

        return 1.0f + 0.03f * count;
    }

    float GetDamageMultiplier(const Inventory& inventory)
    {
        const int count = std::clamp(
            inventory.highlighterCount, 0, MAX_ITEM_COUNT
        );

        return 1.0f + 0.10f * count;
    }

    float GetExperienceMultiplier(const Inventory& inventory)
    {
        const int count = std::clamp(
            inventory.pastExamCount, 0, MAX_ITEM_COUNT
        );

        return 1.0f + 0.03f * count;
    }

    float CalculateSpeed(float baseSpeed, const Inventory& inventory)
    {
        return baseSpeed * GetSpeedMultiplier(inventory);
    }

    float CalculateDamage(float baseDamage, const Inventory& inventory)
    {
        return baseDamage * GetDamageMultiplier(inventory);
    }

    float CalculateExperience(
        float baseExperience,
        const Inventory& inventory
    )
    {
        return baseExperience * GetExperienceMultiplier(inventory);
    }

    void ApplyMealHealing(float& hp)
    {
        // HP를 20 회복하되 최대 100으로 제한
        hp = std::min(hp + 20.0f, 100.0f);
    }
        bool TryDropMeal(
        MealDropSystem& system,
        Vector2 enemyPosition,
        int roll
    )
    {
        // 1~100 중 1~10이면 드롭 성공
        if (roll < 1 || roll > 10)
        {
            return false;
        }

        // 비어 있는 슬롯에만 생성: 최대 2개 유지
        for (MealDrop& drop : system.drops)
        {
            if (!drop.active)
            {
                drop.position = enemyPosition;
                drop.active = true;
                return true;
            }
        }

        return false;
    }

    bool OnEnemyDefeated(
        MealDropSystem& system,
        Vector2 enemyPosition
    )
    {
        return TryDropMeal(
            system,
            enemyPosition,
            GetRandomValue(1, 100)
        );
    }

    int CollectMeals(
        MealDropSystem& system,
        Vector2 playerPosition,
        float playerCollisionRadius,
        float& hp
    )
    {
        // 사망한 플레이어 또는 잘못된 충돌 반지름은 처리하지 않음
        if (hp <= 0.0f || playerCollisionRadius < 0.0f)
        {
            return 0;
        }

        int collected = 0;

        for (MealDrop& drop : system.drops)
        {
            if (!drop.active)
            {
                continue;
            }

            if (CheckCollisionCircles(
                    playerPosition,
                    playerCollisionRadius,
                    drop.position,
                    MEAL_RADIUS))
            {
                ApplyMealHealing(hp);
                drop.active = false;
                ++collected;
            }
        }

        return collected;
    }
}