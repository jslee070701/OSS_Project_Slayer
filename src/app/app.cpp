
#include "app/app.h"

#include "screens/ui_screen.h"

#include "monsters/combat.h"

#include <cmath>

// 게임 시작 시 상태 초기화
static void ResetGame(App& app)
{
    app.player = CreatePlayer({
        app.screenWidth / 2.0f,
        app.screenHeight / 2.0f
    });

    // Core 모듈의 게임 진행 상태 초기화
    app.progress = GameProgress{};

    app.defeatedCount = 0;
    app.spawnTimer = 0.0f;

    // 패시브 아이템 관련 데이터 초기화
    app.inventory = passive_items::Inventory{};
    app.meals = passive_items::MealDropSystem{};
    app.rewardMessage = passive_items::RewardMessage{};

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
        app.shouldExit = true;
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

                int expAmount = static_cast<int>(gainedExp + 0.5f);

                // Player 모듈을 통한 경험치 획득 및 레벨업 처리
                if (GainExperience(app.player, expAmount))
                {
                    // 레벨업에 성공하면 Core 모듈의 스테이지 진행
                    AdvanceStage(app.progress);

                    // 레벨업 보상 지급
                    passive_items::LevelUpReward reward =
                        passive_items::OnLevelUp(
                            app.inventory,
                            app.player.hp,
                            app.player.level
                        );

                    // 보상 획득 문구 설정
                    passive_items::ShowRewardMessage(
                        app.rewardMessage,
                        reward
                    );
                }

                // 몬스터 사망 위치에 학식 드롭 시도
                passive_items::OnEnemyDefeated(
                    app.meals,
                    hitResult.position
                );
            }
        }
    }
}

// 게임 진행 업데이트
static void UpdateGame(App& app, float deltaTime)
{
    // 보상 문구 표시 시간 갱신
    passive_items::UpdateRewardMessage(
        app.rewardMessage,
        deltaTime
    );

    // 플레이어 이동
    UpdatePlayer(
    app.player,
    deltaTime,
    app.screenWidth,
    app.screenHeight,
    passive_items::GetSpeedMultiplier(app.inventory)
);

    // 몬스터 생성
    app.spawnTimer += deltaTime;

    if (app.spawnTimer >= SPAWN_INTERVAL)
    {
        SpawnRandomMonster(
            app.monsters,
            app.screenWidth,
            app.screenHeight,
            app.progress.currentStage
        );

        app.spawnTimer -= SPAWN_INTERVAL;
    }

    // 몬스터 이동
    UpdateMonsters(
        app.monsters,
        app.player.position,
        deltaTime
    );

    // 플레이어와 몬스터의 충돌
    float damageTaken = ResolvePlayerMonsterCombat(
        app.monsters,
        app.player.position,
        app.player.radius
    );

    // Player 모듈을 통한 체력 감소 및 사망 처리
    DamagePlayer(app.player, damageTaken);

    // 플레이어 사망 시 게임 오버 처리
    if (!app.player.isAlive)
    {
        SetGameOver(app.progress);
        app.backToMain = false;
        app.currentScreen = APP_SCREEN_RESULT;
        return;
    }

    // 마우스 좌클릭으로 투사체 발사
    SpawnProjectile(app);

    // 투사체 이동 및 공격
    UpdateProjectiles(app, deltaTime);

    // 학식 획득 및 HP 회복
    passive_items::CollectMeals(
        app.meals,
        app.player.position,
        app.player.radius,
        app.player.hp
    );
}

// App 업데이트
void UpdateApp(
    App& app,
    float deltaTime,
    int screenWidth,
    int screenHeight
)
{
    (void)screenWidth;
    (void)screenHeight;

    switch (app.currentScreen)
    {
        case APP_SCREEN_NAME_INPUT:
            if (app.isInputComplete)
            {
                app.currentScreen = APP_SCREEN_MAIN_MENU;
                app.selectedMenu = 0;
            }
            break;

        case APP_SCREEN_MAIN_MENU:
            UpdateMainMenu(app);
            break;

        case APP_SCREEN_LEADERBOARD:
            if (app.backToMain)
            {
                app.backToMain = false;
                app.currentScreen = APP_SCREEN_MAIN_MENU;
            }
            break;

        case APP_SCREEN_GAME:
            UpdateGame(app, deltaTime);
            break;

        case APP_SCREEN_RESULT:
            if (app.backToMain)
            {
                app.backToMain = false;
                app.currentScreen = APP_SCREEN_MAIN_MENU;
            }
            break;
    }
}

// 게임 오브젝트 출력
static void DrawGameObjects(const App& app)
{
    // 학식 출력
    passive_items::DrawMeals(app.meals);

    // 몬스터 출력
    DrawMonsters(app.monsters);

    // 투사체 출력
    for (int i = 0; i < MAX_PROJECTILES; ++i)
    {
        const AppProjectile& projectile = app.projectiles[i];

        if (projectile.active)
        {
            DrawCircleV(
                projectile.position,
                projectile.radius,
                YELLOW
            );
        }
    }

    // 플레이어 출력
    DrawPlayer(app.player);

    // 플레이어 HP 바
    float hpRatio = 0.0f;

    if (app.player.maxHp > 0.0f)
    {
        hpRatio = app.player.hp / app.player.maxHp;
    }

    if (hpRatio < 0.0f) hpRatio = 0.0f;
    if (hpRatio > 1.0f) hpRatio = 1.0f;

    float hpBarWidth = 50.0f;
    float hpBarHeight = 6.0f;

    DrawRectangle(
        (int)(app.player.position.x - hpBarWidth / 2),
        (int)(app.player.position.y + app.player.radius + 6),
        (int)hpBarWidth,
        (int)hpBarHeight,
        WHITE
    );

    DrawRectangle(
        (int)(app.player.position.x - hpBarWidth / 2),
        (int)(app.player.position.y + app.player.radius + 6),
        (int)(hpBarWidth * hpRatio),
        (int)hpBarHeight,
        RED
    );

    // 게임 정보 출력
    DrawText(
        TextFormat(
            "HP: %.0f | Stage: %d | Killed: %d",
            app.player.hp,
            app.progress.currentStage,
            app.defeatedCount
        ),
        20,
        80,
        18,
        WHITE
    );

    DrawText(
        TextFormat(
            "Active Monsters: %d",
            GetActiveMonsterCount(app.monsters)
        ),
        20,
        105,
        18,
        YELLOW
    );
}

// App 화면 출력
void DrawApp(App& app)
{
    switch (app.currentScreen)
    {
        case APP_SCREEN_NAME_INPUT:
            DrawNameInputScreen(
                app.playerName,
                10,
                &app.isInputComplete
            );
            break;

        case APP_SCREEN_MAIN_MENU:
            DrawMainScreen(
                &app.selectedMenu,
                app.playerName
            );
            break;

        case APP_SCREEN_LEADERBOARD:
            DrawLeaderboardScreen(
                &app.backToMain,
                app.playerName,
                TextFormat("Stage %d", app.progress.currentStage),
                app.defeatedCount
            );
            break;

        case APP_SCREEN_GAME:
            // UI 함수가 배경을 지우므로 게임 오브젝트보다 먼저 호출
            DrawGameScreen(
                app.playerName,
                app.player.experience,
                app.player.requiredExperience,
                app.progress.currentStage
            );

            DrawGameObjects(app);

            // 보상 획득 문구 출력
            passive_items::DrawRewardMessage(
                app.rewardMessage,
                app.screenWidth,
                app.screenHeight
            );

            break;

        case APP_SCREEN_RESULT:
        {
            // Core 모듈의 게임 상태를 결과 화면에 전달
            bool isClear =
                app.progress.gameState == GameState::GAME_CLEAR;

            DrawResultScreen(
                isClear,
                &app.backToMain,
                app.playerName,
                TextFormat("Stage %d", app.progress.currentStage),
                app.defeatedCount
            );

            break;
        }
    }
}

// App 종료 및 정리
void ShutdownApp(App& app)
{
    (void)app;
}
