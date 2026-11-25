
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

    void DrawMeals(const MealDropSystem& system)
    {
        // 학식은 초록색 원으로 임시 표시
        for (const MealDrop& drop : system.drops)
        {
            if (!drop.active)
            {
                continue;
            }

            DrawCircleV(drop.position, MEAL_RADIUS, GREEN);
            DrawText(
                "+20",
                static_cast<int>(drop.position.x) - 15,
                static_cast<int>(drop.position.y) - 30,
                18,
                GREEN
            );
        }
    }

    int GetAvailableItems(const Inventory& inventory, ItemType out[3])
    {
        int count = 0;

        // 최대 개수인 아이템은 후보에서 제외
        if (inventory.coffeeCount < MAX_ITEM_COUNT)
        {
            out[count++] = ItemType::Coffee;
        }

        if (inventory.highlighterCount < MAX_ITEM_COUNT)
        {
            out[count++] = ItemType::Highlighter;
        }

        if (inventory.pastExamCount < MAX_ITEM_COUNT)
        {
            out[count++] = ItemType::PastExam;
        }

        return count;
    }

    LevelUpReward GrantLevelUpReward(
        Inventory& inventory,
        float& hp,
        int newLevel,
        int pick
    )
    {
        LevelUpReward reward{};

        // 레벨 13이면 아이템을 지급하지 않음
        if (newLevel >= NO_REWARD_LEVEL)
        {
            return reward;
        }

        ItemType available[3];
        const int count = GetAvailableItems(inventory, available);

        // 모든 아이템이 최대 개수면 학식을 드롭 없이 즉시 지급
        if (count == 0)
        {
            ApplyMealHealing(hp);
            reward.kind = RewardKind::Meal;
            return reward;
        }

        // 잘못된 순번이 들어와도 후보 범위 안으로 보정
        if (pick < 0)
        {
            pick = 0;
        }

        reward.item = available[pick % count];
        reward.kind = RewardKind::Item;
        TryAcquireItem(inventory, reward.item);

        return reward;
    }

    LevelUpReward OnLevelUp(Inventory& inventory, float& hp, int newLevel)
    {
        ItemType available[3];
        const int count = GetAvailableItems(inventory, available);

        // 지급 가능한 아이템 중 하나를 같은 확률로 선택
        const int pick = count > 0 ? GetRandomValue(0, count - 1) : 0;

        return GrantLevelUpReward(inventory, hp, newLevel, pick);
    }

    const char* GetRewardText(const LevelUpReward& reward)
    {
        if (reward.kind == RewardKind::Meal)
        {
            return "Meal acquired! (+20 HP)";
        }

        if (reward.kind == RewardKind::None)
        {
            return "";
        }

        switch (reward.item)
        {
        case ItemType::Coffee:
            return "Coffee acquired!";

        case ItemType::Highlighter:
            return "Highlighter acquired!";

        case ItemType::PastExam:
            return "Past exam acquired!";
        }

        return "";
    }

    void ShowRewardMessage(RewardMessage& message, const LevelUpReward& reward)
    {
        // 지급이 없으면 기존 문구와 표시 시간 초기화
        if (reward.kind == RewardKind::None)
        {
            message.text = "";
            message.timer = 0.0f;
            return;
        }

        message.text = GetRewardText(reward);
        message.timer = REWARD_MESSAGE_TIME;
    }

    void UpdateRewardMessage(RewardMessage& message, float deltaTime)
    {
        if (message.timer > 0.0f)
        {
            message.timer -= deltaTime;
        }
    }

    void DrawRewardMessage(
        const RewardMessage& message,
        int screenWidth,
        int screenHeight
    )
    {
        if (message.timer <= 0.0f)
        {
            return;
        }

        // 화면 중앙에 문구 출력
        const int fontSize = 32;
        const int textWidth = MeasureText(message.text, fontSize);

        DrawText(
            message.text,
            (screenWidth - textWidth) / 2,
            screenHeight / 2 - fontSize / 2,
            fontSize,
            GOLD
        );
    }
}
