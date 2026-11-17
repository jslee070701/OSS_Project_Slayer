#ifndef UI_SCREEN_H
#define UI_SCREEN_H

#include "raylib.h"

void DrawNameInputScreen(char *nameBuffer, int maxInputLength, bool *isInputComplete);
void DrawMainScreen(int *selectedMenu, const char *playerName);
void DrawLeaderboardScreen(bool *backToMain, const char *playerName, const char *clearedStage, int monstersDefeated);
void DrawGameScreen(bool *isPaused, const char *playerName);

#endif