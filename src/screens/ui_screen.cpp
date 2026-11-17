#include "screens/ui_screen.h"
#include <cstdio>
#include <cstring>

// 1. 이름 입력 화면
void DrawNameInputScreen(char *nameBuffer, int maxInputLength, bool *isInputComplete) {
    ClearBackground(Color{ 20, 24, 33, 255 });

    DrawText("EXAM SEASON SURVIVORS", GetScreenWidth() / 2 - 210, 80, 28, RED);
    DrawText("[SURVIVAL REGISTRATION FORM]", GetScreenWidth() / 2 - 140, 120, 16, YELLOW);
    DrawText("Enter Lowercase English Name (1-10 chars)", GetScreenWidth() / 2 - 200, 170, 18, LIGHTGRAY);

    Rectangle textBox = { (float)(GetScreenWidth() / 2 - 150), 220, 300, 50 };
    Vector2 mousePos = GetMousePosition();

    static bool isTextBoxActive = false;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePos, textBox)) {
            isTextBoxActive = true;
        } else {
            isTextBoxActive = false;
        }
    }

    if (isTextBoxActive) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key == 13 || key == 10) {
                int len = strlen(nameBuffer);
                if (len >= 1 && len < maxInputLength) {
                    *isInputComplete = true;
                    isTextBoxActive = false;
                }
                break;
            }

            if (key >= 'a' && key <= 'z') {
                int currentLen = strlen(nameBuffer);
                if (currentLen < maxInputLength - 1) {
                    nameBuffer[currentLen] = (char)key;
                    nameBuffer[currentLen + 1] = '\0';
                }
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            int currentLen = strlen(nameBuffer);
            if (currentLen > 0) {
                nameBuffer[currentLen - 1] = '\0';
            }
        }

        if (IsKeyPressed(KEY_ENTER)) {
            int len = strlen(nameBuffer);
            if (len >= 1 && len < maxInputLength) {
                *isInputComplete = true;
                isTextBoxActive = false;
            }
        }
    }

    DrawRectangleRec(textBox, Color{ 40, 44, 52, 255 });
    DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, isTextBoxActive ? YELLOW : DARKGRAY);
    DrawText(nameBuffer, (int)textBox.x + 15, (int)textBox.y + 15, 22, WHITE);

    Rectangle btnBounds = { (float)(GetScreenWidth() / 2 - 75), 320, 150, 40 };
    bool btnState = CheckCollisionPointRec(mousePos, btnBounds);

    DrawRectangleRec(btnBounds, btnState ? DARKGREEN : Color{ 0, 120, 80, 255 });
    DrawText("Confirm", (int)btnBounds.x + 40, (int)btnBounds.y + 10, 20, WHITE);

    if (btnState && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int len = strlen(nameBuffer);
        if (len >= 1 && len < maxInputLength) {
            *isInputComplete = true;
            isTextBoxActive = false;
        }
    }
}

// 2. 메인 화면
void DrawMainScreen(int *selectedMenu, const char *playerName) {
    ClearBackground(Color{ 25, 30, 40, 255 });

    DrawText(TextFormat("Player: %s", playerName[0] != '\0' ? playerName : "None"), 20, 20, 18, GOLD);
    DrawText("EXAM SEASON SURVIVORS", GetScreenWidth() / 2 - 210, 120, 30, RAYWHITE);

    Rectangle startBtn = { (float)(GetScreenWidth() / 2 - 110), 220, 220, 50 };
    Rectangle rankBtn  = { (float)(GetScreenWidth() / 2 - 110), 290, 220, 50 };
    Rectangle exitBtn  = { (float)(GetScreenWidth() / 2 - 110), 360, 220, 50 };

    Vector2 mousePos = GetMousePosition();

    bool startHover = CheckCollisionPointRec(mousePos, startBtn);
    DrawRectangleRec(startBtn, startHover ? Color{ 0, 160, 80, 255 } : Color{ 0, 120, 60, 255 });
    DrawText("Start Survival", (int)startBtn.x + 35, (int)startBtn.y + 15, 18, WHITE);
    if (startHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *selectedMenu = 1;
    }

    bool rankHover = CheckCollisionPointRec(mousePos, rankBtn);
    DrawRectangleRec(rankBtn, rankHover ? Color{ 200, 130, 0, 255 } : Color{ 180, 100, 0, 255 });
    DrawText("Leaderboard", (int)rankBtn.x + 45, (int)rankBtn.y + 15, 18, WHITE);
    if (rankHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *selectedMenu = 2;
    }

    bool exitHover = CheckCollisionPointRec(mousePos, exitBtn);
    DrawRectangleRec(exitBtn, exitHover ? Color{ 180, 40, 40, 255 } : Color{ 140, 30, 30, 255 });
    DrawText("Give Up (Exit)", (int)exitHover ? (int)exitBtn.x + 48 : (int)exitBtn.x + 48, (int)exitBtn.y + 15, 18, WHITE);
    if (exitHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *selectedMenu = 3;
    }
}

// 3. 리더보드 화면
void DrawLeaderboardScreen(bool *backToMain, const char *playerName, const char *clearedStage, int monstersDefeated) {
    ClearBackground(Color{ 20, 24, 33, 255 });

    DrawText("HALL OF SURVIVORS", GetScreenWidth() / 2 - 130, 50, 26, GOLD);

    DrawRectangle(100, 110, 1080, 40, Color{ 35, 40, 50, 255 });
    DrawText("Rank", 130, 122, 16, LIGHTGRAY);
    DrawText("Player Name", 260, 122, 16, LIGHTGRAY);
    DrawText("Cleared Stage", 560, 122, 16, LIGHTGRAY);
    DrawText("Monsters Defeated", 860, 122, 16, LIGHTGRAY);

    DrawRectangle(100, 160, 1080, 45, Color{ 30, 35, 45, 255 });
    DrawText("1", 130, 175, 16, YELLOW);
    DrawText(playerName[0] != '\0' ? playerName : "player1", 260, 175, 16, WHITE);
    DrawText(clearedStage, 560, 175, 16, GREEN);
    DrawText(TextFormat("%d", monstersDefeated), 860, 175, 16, WHITE);

    DrawRectangle(100, 215, 1080, 45, Color{ 30, 35, 45, 255 });
    DrawText("2", 130, 230, 16, LIGHTGRAY);
    DrawText("player2", 260, 230, 16, WHITE);
    DrawText("Midterm 1 Weeks Before", 560, 230, 16, LIGHTGRAY);
    DrawText("30", 860, 230, 16, LIGHTGRAY);

    Rectangle backBtn = { (float)(GetScreenWidth() / 2 - 75), 560, 150, 40 };
    Vector2 mousePos = GetMousePosition();
    bool btnHover = CheckCollisionPointRec(mousePos, backBtn);
    DrawRectangleRec(backBtn, btnHover ? DARKGRAY : Color{ 60, 65, 75, 255 });
    DrawText("Back", (int)backBtn.x + 55, (int)backBtn.y + 10, 20, WHITE);

    if (btnHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *backToMain = true;
    }
}

// 4. 게임 화면
void DrawGameScreen(bool *isPaused, const char *playerName) {
    ClearBackground(DARKGRAY);

    DrawRectangle(GetScreenWidth() / 2 - 250, 20, 500, 20, LIGHTGRAY);
    DrawRectangle(GetScreenWidth() / 2 - 250, 20, 250, 20, GREEN);
    DrawText("EXP", GetScreenWidth() / 2 - 280, 20, 16, WHITE);

    DrawText("Stage: Midterm 5 Weeks Before", GetScreenWidth() / 2 - 120, 45, 16, YELLOW);
    DrawText(TextFormat("Player: %s", playerName[0] != '\0' ? playerName : "Player1"), 20, 20, 18, WHITE);

    Rectangle pauseBtn = { 20, 48, 80, 30 };
    Vector2 mousePos = GetMousePosition();
    bool btnHover = CheckCollisionPointRec(mousePos, pauseBtn);
    DrawRectangleRec(pauseBtn, btnHover ? GRAY : LIGHTGRAY);
    DrawText("Pause", (int)pauseBtn.x + 15, (int)pauseBtn.y + 5, 16, BLACK);

    if (btnHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *isPaused = !(*isPaused);
    }

    DrawText("Monsters Defeated: 12", GetScreenWidth() - 220, 20, 18, YELLOW);

    DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 20, BLUE);
    DrawRectangle(GetScreenWidth() / 2 - 25, GetScreenHeight() / 2 + 25, 50, 10, RED);
}