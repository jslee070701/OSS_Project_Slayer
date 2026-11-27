#include "items/passive_items.h"

#include <cmath>
#include <iostream>
#include <string>

namespace
{
    // 테스트용 임시 플레이어
    struct TestPlayer
    {
        float hp = 70.0f;
        float baseSpeed = 10.0f;
        float baseDamage = 50.0f;
        float baseExperience = 10.0f;
        passive_items::Inventory inventory;
    };

    int passed = 0;
    int failed = 0;

    void Check(bool condition, const char* description)
    {
        if (condition)
        {
            std::cout << "[PASS] " << description << '\n';
            ++passed;
        }
        else
        {
            std::cout << "[FAIL] " << description << '\n';
            ++failed;
        }
    }

    // 실수 계산 결과 비교
    bool NearlyEqual(float actual, float expected)
    {
        return std::fabs(actual - expected) < 0.0001f;
    }
}

int main()
{
    using namespace passive_items;

    TestPlayer player{};

    // 아이템이 없으면 기본 수치 유지
    Check(
        NearlyEqual(
            CalculateSpeed(player.baseSpeed, player.inventory), 10.0f
        ),
        "No coffee: speed = 10"
    );

    Check(
        NearlyEqual(
            CalculateDamage(player.baseDamage, player.inventory), 50.0f
        ),
        "No highlighter: damage = 50"
    );

    Check(
        NearlyEqual(
            CalculateExperience(
                player.baseExperience, player.inventory
            ),
            10.0f
        ),
        "No past exam: XP gain = 10"
    );

    // 각 아이템 1개 획득
    Check(
        TryAcquireItem(player.inventory, ItemType::Coffee),
        "First coffee acquired"
    );

    Check(
        TryAcquireItem(player.inventory, ItemType::Highlighter),
        "First highlighter acquired"
    );

    Check(
        TryAcquireItem(player.inventory, ItemType::PastExam),
        "First past exam acquired"
    );

    Check(
        NearlyEqual(
            CalculateSpeed(player.baseSpeed, player.inventory), 10.3f
        ),
        "One coffee: speed 10 -> 10.3"
    );

    Check(
        NearlyEqual(
            CalculateDamage(player.baseDamage, player.inventory), 55.0f
        ),
        "One highlighter: damage 50 -> 55"
    );

    Check(
        NearlyEqual(
            CalculateExperience(
                player.baseExperience, player.inventory
            ),
            10.3f
        ),
        "One past exam: XP gain 10 -> 10.3"
    );

    // 아이템을 각각 5개까지 획득
    for (int count = 2; count <= MAX_ITEM_COUNT; ++count)
    {
        Check(
            TryAcquireItem(player.inventory, ItemType::Coffee),
            "Additional coffee acquired"
        );

        Check(
            TryAcquireItem(player.inventory, ItemType::Highlighter),
            "Additional highlighter acquired"
        );

        Check(
            TryAcquireItem(player.inventory, ItemType::PastExam),
            "Additional past exam acquired"
        );
    }

    // 최대 중첩 효과 확인: 합연산 기준
    Check(
        NearlyEqual(
            CalculateSpeed(player.baseSpeed, player.inventory), 11.5f
        ),
        "Five coffees: speed = 11.5 (+15%)"
    );

    Check(
        NearlyEqual(
            CalculateDamage(player.baseDamage, player.inventory), 75.0f
        ),
        "Five highlighters: damage = 75 (+50%)"
    );

    Check(
        NearlyEqual(
            CalculateExperience(
                player.baseExperience, player.inventory
            ),
            11.5f
        ),
        "Five past exams: XP gain = 11.5 (+15%)"
    );

    // 여섯 번째 아이템 획득 거부
    Check(
        !TryAcquireItem(player.inventory, ItemType::Coffee),
        "Sixth coffee rejected"
    );

    Check(
        !TryAcquireItem(player.inventory, ItemType::Highlighter),
        "Sixth highlighter rejected"
    );

    Check(
        !TryAcquireItem(player.inventory, ItemType::PastExam),
        "Sixth past exam rejected"
    );

    Check(
        player.inventory.coffeeCount == 5 &&
        player.inventory.highlighterCount == 5 &&
        player.inventory.pastExamCount == 5,
        "All item counts remain at five"
    );

    // 같은 기본 수치로 재계산해도 보너스가 중복 적용되지 않음
    Check(
        NearlyEqual(
            CalculateSpeed(player.baseSpeed, player.inventory), 11.5f
        ),
        "Repeated calculation: speed remains 11.5"
    );

    // 학식 회복량 및 최대 HP 확인
    ApplyMealHealing(player.hp);
    Check(NearlyEqual(player.hp, 90.0f), "Meal: HP 70 -> 90");

    ApplyMealHealing(player.hp);
    Check(NearlyEqual(player.hp, 100.0f), "Meal: HP 90 -> 100");

    ApplyMealHealing(player.hp);
    Check(NearlyEqual(player.hp, 100.0f), "Meal: HP stays at 100");

        // 학식 드롭 및 충돌 획득 테스트
    MealDropSystem meals{};

    const Vector2 firstPosition = { 100.0f, 100.0f };
    const Vector2 secondPosition = { 300.0f, 300.0f };

    // 확률 판정 경계 및 잘못된 입력 확인
    Check(
        !TryDropMeal(meals, firstPosition, 0),
        "Invalid roll 0 rejected"
    );

    Check(
        !TryDropMeal(meals, firstPosition, 11),
        "Roll 11: no meal dropped"
    );

    Check(
        !TryDropMeal(meals, firstPosition, 100),
        "Roll 100: no meal dropped"
    );

    Check(
        !TryDropMeal(meals, firstPosition, 101),
        "Invalid roll 101 rejected"
    );

    // 두 개까지 드롭 가능
    Check(
        TryDropMeal(meals, firstPosition, 1),
        "Roll 1: first meal dropped"
    );

    Check(
        NearlyEqual(meals.drops[0].position.x, firstPosition.x) &&
        NearlyEqual(meals.drops[0].position.y, firstPosition.y),
        "Meal appears at enemy position"
    );

    Check(
        TryDropMeal(meals, secondPosition, 10),
        "Roll 10: second meal dropped"
    );

    Check(
        !TryDropMeal(meals, firstPosition, 5),
        "Third meal rejected: two-drop limit"
    );

    // 멀리 떨어진 플레이어는 획득하지 않음
    player.hp = 70.0f;

    Check(
        CollectMeals(
            meals, Vector2{ 600.0f, 600.0f }, 20.0f, player.hp
        ) == 0 &&
        NearlyEqual(player.hp, 70.0f),
        "Distant player: no pickup or healing"
    );

    // 첫 번째 학식 획득 후 제거
    Check(
        CollectMeals(meals, firstPosition, 20.0f, player.hp) == 1 &&
        NearlyEqual(player.hp, 90.0f) &&
        !meals.drops[0].active &&
        meals.drops[1].active,
        "First pickup: HP 70 -> 90, meal removed"
    );

    Check(
        CollectMeals(meals, firstPosition, 20.0f, player.hp) == 0 &&
        NearlyEqual(player.hp, 90.0f),
        "Collected meal cannot heal twice"
    );

    // 두 번째 학식은 최대 HP까지만 회복
    Check(
        CollectMeals(meals, secondPosition, 20.0f, player.hp) == 1 &&
        NearlyEqual(player.hp, 100.0f) &&
        !meals.drops[1].active,
        "Second pickup: HP capped at 100"
    );

    // 획득 후 비워진 슬롯 재사용
    Check(
        TryDropMeal(meals, firstPosition, 5),
        "Empty meal slot can be reused"
    );

    Check(
        CollectMeals(meals, firstPosition, 20.0f, player.hp) == 1 &&
        NearlyEqual(player.hp, 100.0f),
        "Full HP: meal consumed without overhealing"
    );

    // 원의 중심이 달라도 충돌 범위가 겹치면 획득
    TryDropMeal(meals, firstPosition, 1);
    player.hp = 50.0f;

    Check(
        CollectMeals(
            meals, Vector2{ 125.0f, 100.0f }, 20.0f, player.hp
        ) == 1 &&
        NearlyEqual(player.hp, 70.0f),
        "Overlapping circles trigger pickup"
    );

    // 사망한 플레이어는 학식으로 부활하지 않음
    TryDropMeal(meals, firstPosition, 1);
    player.hp = 0.0f;

    Check(
        CollectMeals(meals, firstPosition, 20.0f, player.hp) == 0 &&
        NearlyEqual(player.hp, 0.0f) &&
        meals.drops[0].active,
        "Dead player cannot collect meals"
    );

    // ITEM_001 레벨업 아이템 지급
    Inventory rewardInventory{};
    float rewardHp = 50.0f;

    LevelUpReward reward = GrantLevelUpReward(
        rewardInventory, rewardHp, 2, 0
    );
    Check(
        reward.kind == RewardKind::Item &&
        reward.item == ItemType::Coffee &&
        rewardInventory.coffeeCount == 1,
        "Level up: pick 0 gives one coffee"
    );

    // 커피를 최대 개수로 채우면 후보에서 제외
    rewardInventory.coffeeCount = MAX_ITEM_COUNT;
    reward = GrantLevelUpReward(rewardInventory, rewardHp, 3, 0);
    Check(
        reward.kind == RewardKind::Item &&
        reward.item == ItemType::Highlighter &&
        rewardInventory.coffeeCount == MAX_ITEM_COUNT,
        "Level up: maxed coffee is skipped"
    );

    // 모든 아이템이 최대 개수면 학식 즉시 지급
    rewardInventory.highlighterCount = MAX_ITEM_COUNT;
    rewardInventory.pastExamCount = MAX_ITEM_COUNT;
    reward = GrantLevelUpReward(rewardInventory, rewardHp, 4, 0);
    Check(
        reward.kind == RewardKind::Meal &&
        NearlyEqual(rewardHp, 70.0f),
        "Level up: all items maxed -> meal heals 50 -> 70"
    );

    // 레벨 13이면 지급 없음
    Inventory level13Inventory{};
    float level13Hp = 50.0f;
    reward = GrantLevelUpReward(level13Inventory, level13Hp, 13, 0);
    Check(
        reward.kind == RewardKind::None &&
        level13Inventory.coffeeCount == 0 &&
        NearlyEqual(level13Hp, 50.0f),
        "Level 13: no item given"
    );

    // 화면 중앙 문구
    Inventory textInventory{};
    float textHp = 100.0f;
    RewardMessage rewardMessage{};

    reward = GrantLevelUpReward(textInventory, textHp, 2, 2);
    ShowRewardMessage(rewardMessage, reward);
    Check(
        std::string(rewardMessage.text) == "Past exam acquired!" &&
        rewardMessage.timer > 0.0f,
        "Reward message shows acquired item"
    );

    UpdateRewardMessage(rewardMessage, REWARD_MESSAGE_TIME + 0.1f);
    Check(
        rewardMessage.timer <= 0.0f,
        "Reward message disappears after its time"
    );

    std::cout << "\nPassed: " << passed
              << "\nFailed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}