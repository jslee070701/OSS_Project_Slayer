#include "raylib.h"
#include "screens/ui_screen.h"

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(
        screenWidth,
        screenHeight,
        "Exam Season Survivors - UI Test"
    );

    SetTargetFPS(60);

    char playerName[16] = "";
    bool isInputComplete = false;
    int selectedMenu = 0;
    bool backToMain = false;
    bool isPaused = false;
    bool isClear = true;

    const char* currentStage = "Midterm D-day";
    int currentMonsters = 45;

    int currentTestScreen = 1;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ONE))
            currentTestScreen = 1;

        if (IsKeyPressed(KEY_TWO))
            currentTestScreen = 2;

        if (IsKeyPressed(KEY_THREE))
            currentTestScreen = 3;

        if (IsKeyPressed(KEY_FOUR))
            currentTestScreen = 4;

        if (IsKeyPressed(KEY_FIVE))
            currentTestScreen = 5;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch (currentTestScreen)
        {
            case 1:
                DrawNameInputScreen(
                    playerName,
                    10,
                    &isInputComplete
                );
                break;

            case 2:
                DrawMainScreen(
                    &selectedMenu,
                    playerName
                );
                break;

            case 3:
                DrawLeaderboardScreen(
                    &backToMain,
                    playerName,
                    currentStage,
                    currentMonsters
                );
                break;

            case 4:
                DrawGameScreen(
                    playerName,
                    0,
                    1
                );
                break;

            case 5:
                DrawResultScreen(
                    &isClear,
                    &backToMain,
                    playerName,
                    currentStage,
                    currentMonsters
                );
                break;
        }

        const char* infoText = TextFormat(
            "Screen: %d | Player: %s",
            currentTestScreen,
            playerName[0] != '\0' ? playerName : "(None)"
        );

        int textWidth = MeasureText(infoText, 18);

        DrawText(
            infoText,
            screenWidth - textWidth - 20,
            screenHeight - 35,
            18,
            RED
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}