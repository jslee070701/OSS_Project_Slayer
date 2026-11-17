#pragma once

namespace passive_items
{
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