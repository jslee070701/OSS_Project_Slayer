#include "app/app.h"

#include "screens/ui_screen.h"
#include "monsters/combat.h"

#include <cmath>
#include <cstdio>

// 게임 시작 시 상태 초기화
static void ResetGame(App& app)
{
    app.player = CreatePlayer({
        app.screenWidth / 2.0f,
        app.screenHeight / 2.0f
    });

    app.playerHp = 100.0f;
    app.playerExp = 0;
    app.defeatedCount = 0;
    app.currentStage = 1;

    app.isPaused = false;
    app.isClear = false;

    app.spawnTimer = 0.0f;

    app.inventory = passive_items::Inventory{};
    app.meals = passive_items::MealDropSystem{};

    for (int i = 0; i < MAX_PROJECTILES; ++i)
    {
        app.projectiles[i] = AppProjectile{};
    }

    InitMonsters(app.monsters);
}

// App 초기화
void InitApp(App& app, int screenWidth, int screenHeight)
{
    app = App{};

    app.screenWidth = screenWidth;
    app.screenHeight = screenHeight;

    app.currentScreen = APP_SCREEN_NAME_INPUT;

    app.playerName[0] = '\0';

    app.isInputComplete = false;
    app.selectedMenu = 0;
    app.backToMain = false;

    ResetGame(app);
}

// 메인 메뉴 선택 처리
static void UpdateMainMenu(App& app)
{
    if (app.selectedMenu == 1)
    {
        ResetGame(app);
        app.currentScreen = APP_SCREEN_GAME;
        app.selectedMenu = 0;
    }
    else if (app.selectedMenu == 2)
    {
        app.currentScreen = APP_SCREEN_LEADERBOARD;
        app.backToMain = false;
        app.selectedMenu = 0;
    }
    else if (app.selectedMenu == 3)
    {
        SetWindowShouldClose(true);
        app.selectedMenu = 0;
    }
}

// 투사체 생성
static void SpawnProjectile(App& app)
{
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return;

    Vector2 mousePosition = GetMousePosition();

    float dx = mousePosition.x - app.player.position.x;
    float dy = mousePosition.y - app.player.position.y;

    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance <= 0.0f)
        return;

    for (int i = 0; i < MAX_PROJECTILES; ++i)
    {
        if (!app.projectiles[i].active)
        {
            float damage = passive_items::CalculateDamage(
                20.0f,
                app.inventory
            );

            app.projectiles[i].position = app.player.position;

            app.projectiles[i].velocity = {
                (dx / distance) * 450.0f,
                (dy / distance) * 450.0f
            };

            app.projectiles[i].radius = 6.0f;
            app.projectiles[i].damage = damage;
            app.projectiles[i].active = true;

            break;
        }
    }
}

// 투사체 업데이트 및 몬스터 공격
static void UpdateProjectiles(App& app, float deltaTime)
{
    for (int i = 0; i < MAX_PROJECTILES; ++i)
    {
        AppProjectile& projectile = app.projectiles[i];

        if (!projectile.active)
            continue;

        projectile.position.x += projectile.velocity.x * deltaTime;
        projectile.position.y += projectile.velocity.y * deltaTime;

        // 화면 밖으로 나간 투사체 제거
        if (
            projectile.position.x < 0 ||
            projectile.position.x > app.screenWidth ||
            projectile.position.y < 0 ||
            projectile.position.y > app.screenHeight
        )
        {
            projectile.active = false;
            continue;
        }

        MonsterHitResult hitResult = ResolveProjectileMonsterCombat(
            app.monsters,
            projectile.position,
            projectile.radius,
            projectile.damage
        );

        if (hitResult.hit)
        {
            projectile.active = false;

            if (hitResult.killed)
            {
                app.defeatedCount++;

                float gainedExp = passive_items::CalculateExperience(
                    static_cast<float>(hitResult.expReward),
                    app.inventory
                );

                app.playerExp += static_cast<int>(gainedExp + 0.5f);

                passive_items::OnEnemyDefeated(
                    app.meals,
                    hitResult.position
                );
            }
        }
    }
}