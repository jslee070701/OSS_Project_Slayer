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

    // 화면에 활성화된 학식 그리기
    void DrawMeals(const MealDropSystem& system);

    //------------------------------------------

    // 이 레벨에 도달하면 아이템을 지급하지 않음
    constexpr int NO_REWARD_LEVEL = 12;

    // 획득 문구 표시 시간(초)
    constexpr float REWARD_MESSAGE_TIME = 2.0f;

    // 레벨업 보상 결과
    enum class RewardKind
    {
        None,   
        Item,   // 패시브 아이템 지급
        Meal    // 모든 아이템 최대 개수: 학식 즉시 지급
    };

    struct LevelUpReward
    {
        RewardKind kind = RewardKind::None;
        ItemType item = ItemType::Coffee;
    };

    // 최대 개수가 아닌 아이템 목록을 out에 채우고 개수 반환
    int GetAvailableItems(const Inventory& inventory, ItemType out[3]);

    // 테스트 가능한 레벨업 보상: pick은 지급 가능한 아이템 중 순번
    LevelUpReward GrantLevelUpReward(
        Inventory& inventory,
        float& hp,
        int newLevel,
        int pick
    );

    // 레벨업 시 호출하는 실제 무작위 보상 함수
    LevelUpReward OnLevelUp(Inventory& inventory, float& hp, int newLevel);

    // 화면 중앙 획득 문구
    struct RewardMessage
    {
        const char* text = "";
        float timer = 0.0f;
    };

    // 보상 결과에 맞는 문구 반환 (예: "Coffee acquired!")
    const char* GetRewardText(const LevelUpReward& reward);

    // 문구 표시 시작
    void ShowRewardMessage(RewardMessage& message, const LevelUpReward& reward);

    // 매 프레임 표시 시간 감소
    void UpdateRewardMessage(RewardMessage& message, float deltaTime);

    // 표시 시간이 남아 있으면 화면 중앙에 출력
    void DrawRewardMessage(
        const RewardMessage& message,
        int screenWidth,
        int screenHeight
    );
}
