#include "items/passive_items.h"

#include <cmath>
#include <iostream>

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

    std::cout << "\nPassed: " << passed
              << "\nFailed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}