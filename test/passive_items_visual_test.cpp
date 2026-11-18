#include "items/passive_items.h"

#include <algorithm>
#include <cmath>
#include <string>

int main()
{
    using namespace passive_items;

    constexpr int screenWidth = 1100;
    constexpr int screenHeight = 720;
    constexpr float playerRadius = 20.0f;

    // 이동 확인을 위한 테스트 전용 속도
    constexpr float baseSpeed = 250.0f;
    constexpr float baseDamage = 50.0f;
    constexpr float baseExperience = 10.0f;

    InitWindow(
        screenWidth,
        screenHeight,
        "Exam Season Survivors - Passive Items Test"
    );
    SetTargetFPS(60);

    Inventory inventory{};
    MealDropSystem meals{};

    Vector2 position = { 550.0f, 480.0f };
    float hp = 70.0f;

    std::string message = "Ready. Press 1, 2 or 3 to acquire items.";

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        // 전체 테스트 상태 초기화
        if (IsKeyPressed(KEY_R))
        {
            inventory = Inventory{};
            meals = MealDropSystem{};
            position = Vector2{ 550.0f, 480.0f };
            hp = 70.0f;
            message = "Test reset.";
        }

        // 패시브 아이템 획득 및 최대 개수 확인
        if (IsKeyPressed(KEY_ONE))
        {
            message = TryAcquireItem(inventory, ItemType::Coffee)
                ? "Coffee acquired: movement speed increased."
                : "Coffee limit reached (5).";
        }

        if (IsKeyPressed(KEY_TWO))
        {
            message = TryAcquireItem(inventory, ItemType::Highlighter)
                ? "Highlighter acquired: damage bonus increased."
                : "Highlighter limit reached (5).";
        }

        if (IsKeyPressed(KEY_THREE))
        {
            message = TryAcquireItem(inventory, ItemType::PastExam)
                ? "Past exam acquired: XP bonus increased."
                : "Past exam limit reached (5).";
        }

        // 회복 확인용 피해: 테스트 중 사망하지 않도록 최소 HP 1
        if (IsKeyPressed(KEY_H))
        {
            hp = std::max(1.0f, hp - 20.0f);
            message = "Test damage: HP reduced by 20 (minimum 1).";
        }

        // 마우스 위치를 임시 몬스터 사망 위치로 사용
        const Vector2 dropPosition = GetMousePosition();

        const bool insidePlayArea =
            dropPosition.x >= MEAL_RADIUS &&
            dropPosition.x <= screenWidth - MEAL_RADIUS &&
            dropPosition.y >= 320.0f + MEAL_RADIUS &&
            dropPosition.y <= screenHeight - MEAL_RADIUS;

        if (IsKeyPressed(KEY_M))
        {
            // 시각 테스트를 위해 드롭 성공값을 직접 전달
            if (!insidePlayArea)
            {
                message = "Move the mouse into the play area first.";
            }
            else
            {
                message = TryDropMeal(meals, dropPosition, 1)
                    ? "Meal placed at mouse position (forced test drop)."
                    : "Meal limit reached (2).";
            }
        }

        if (IsKeyPressed(KEY_K))
        {
            // 실제 10% 확률 드롭 함수 테스트
            if (!insidePlayArea)
            {
                message = "Move the mouse into the play area first.";
            }
            else
            {
                message = OnEnemyDefeated(meals, dropPosition)
                    ? "Simulated enemy defeated: meal dropped!"
                    : "No drop: random roll failed or meal slots full.";
            }
        }

        // 임시 플레이어 이동
        Vector2 direction = { 0.0f, 0.0f };

        if (IsKeyDown(KEY_W)) direction.y -= 1.0f;
        if (IsKeyDown(KEY_S)) direction.y += 1.0f;
        if (IsKeyDown(KEY_A)) direction.x -= 1.0f;
        if (IsKeyDown(KEY_D)) direction.x += 1.0f;

        const float length = std::sqrt(
            direction.x * direction.x +
            direction.y * direction.y
        );

        if (length > 0.0f)
        {
            direction.x /= length;
            direction.y /= length;
        }

        const float speed = CalculateSpeed(baseSpeed, inventory);

        position.x += direction.x * speed * deltaTime;
        position.y += direction.y * speed * deltaTime;

        // 상단 정보 영역 밖에서만 이동
        position.x = std::clamp(
            position.x, playerRadius, screenWidth - playerRadius
        );

        position.y = std::clamp(
            position.y,
            320.0f + playerRadius,
            screenHeight - playerRadius - 30.0f
        );

        // 충돌한 학식 회수 및 회복
        const int collected = CollectMeals(
            meals, position, playerRadius, hp
        );

        if (collected > 0)
        {
            message = "Meal collected: healed up to the 100 HP cap.";
        }

        int activeMeals = 0;

        for (const MealDrop& drop : meals.drops)
        {
            if (drop.active)
            {
                ++activeMeals;
            }
        }

        // 화면 출력
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(0, 0, screenWidth, 320, DARKGRAY);

        DrawText("PASSIVE ITEMS - VISUAL TEST", 20, 15, 26, WHITE);

        DrawText(
            "WASD: move | 1: coffee | 2: highlighter | 3: past exam",
            20, 55, 20, WHITE
        );

        DrawText(
            "H: take damage | M: force meal at mouse | K: random drop",
            20, 85, 20, WHITE
        );

        DrawText(
            "R: reset | ESC: close | Place meals below this panel",
            20, 115, 20, LIGHTGRAY
        );

        DrawText(
            TextFormat(
                "Coffee: %d/5 | Speed: %.1f -> %.1f px/s",
                inventory.coffeeCount, baseSpeed, speed
            ),
            20, 155, 20, SKYBLUE
        );

        DrawText(
            TextFormat(
                "Highlighter: %d/5 | Damage: %.1f -> %.1f",
                inventory.highlighterCount,
                baseDamage,
                CalculateDamage(baseDamage, inventory)
            ),
            20, 185, 20, YELLOW
        );

        DrawText(
            TextFormat(
                "Past exam: %d/5 | XP per kill: %.1f -> %.2f",
                inventory.pastExamCount,
                baseExperience,
                CalculateExperience(baseExperience, inventory)
            ),
            20, 215, 20, GREEN
        );

        DrawText(
            TextFormat(
                "HP: %.0f/100 | Meals on ground: %d/2",
                hp, activeMeals
            ),
            20, 245, 20, WHITE
        );

        DrawText(message.c_str(), 20, 280, 18, ORANGE);

        // 학식은 초록색 원으로 임시 표시
        for (const MealDrop& drop : meals.drops)
        {
            if (drop.active)
            {
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

        // 임시 플레이어와 HP 바
        DrawCircleV(position, playerRadius, SKYBLUE);

        const int barX = static_cast<int>(position.x) - 30;
        const int barY = static_cast<int>(position.y) + 28;

        DrawRectangle(barX, barY, 60, 8, DARKGRAY);
        DrawRectangle(
            barX,
            barY,
            static_cast<int>(60.0f * hp / 100.0f),
            8,
            GREEN
        );
        DrawRectangleLines(barX, barY, 60, 8, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}