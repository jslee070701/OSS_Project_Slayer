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