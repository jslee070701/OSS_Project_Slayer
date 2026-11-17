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