#ifndef APP_H
#define APP_H

#include "raylib.h"
#include "player/player.h"
#include "monsters/monster.h"
#include "items/passive_items.h"

// 화면 상태
enum AppScreen
{
    APP_SCREEN_NAME_INPUT,
    APP_SCREEN_MAIN_MENU,
    APP_SCREEN_LEADERBOARD,
    APP_SCREEN_GAME,
    APP_SCREEN_RESULT
};

// 게임 상태
struct App
{
    // 화면 정보
    AppScreen currentScreen;

    // 플레이어 정보
    char playerName[16];
    Player player;

    // 게임 진행 정보
    Monster monsters[MAX_MONSTERS];
    passive_items::Inventory inventory;
    passive_items::MealDropSystem meals;

    // 플레이어 게임 데이터
    float playerHp;
    int playerExp;
    int defeatedCount;
    int currentStage;

    // 게임 상태
    bool isPaused;
    bool isClear;

    // 몬스터 생성 시간
    float spawnTimer;
};

// App 초기화
void InitApp(App& app, int screenWidth, int screenHeight);

// App 업데이트
void UpdateApp(App& app, float deltaTime, int screenWidth, int screenHeight);

// App 화면 출력
void DrawApp(App& app);

// App 종료 및 상태 정리
void ShutdownApp(App& app);

#endif