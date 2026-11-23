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