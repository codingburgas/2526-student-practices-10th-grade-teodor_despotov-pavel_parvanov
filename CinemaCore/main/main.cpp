#include "raylib.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Movie Ticket System - Raylib Menu");

    const char* options[] = {
        "1. Search Movies",
        "2. View Showtimes",
        "3. Make a Booking",
        "4. Select Seats",
        "5. Payment",
        "6. Admin Panel",
        "7. Exit"
    };
    int optionCount = 7;
    int selectedOption = -1;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // ?????? ?? ????? (???????? ?? ??????? ????? ???????)
        Vector2 mousePoint = GetMousePosition();
        selectedOption = -1;

        for (int i = 0; i < optionCount; i++) {
            Rectangle itemRect = { 250, (float)(150 + i * 50), 300, 40 };
            if (CheckCollisionPointRec(mousePoint, itemRect)) {
                selectedOption = i;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (i == 6) return 0; // Exit
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Movie Ticket System Menu", 220, 50, 30, DARKGRAY);
        DrawLine(200, 90, 600, 90, LIGHTGRAY);

        for (int i = 0; i < optionCount; i++) {
            Color textColor = (selectedOption == i) ? MAROON : DARKBLUE;
            DrawText(options[i], 270, 160 + i * 50, 20, textColor);

            if (selectedOption == i) {
                DrawRectangleLines(250, 150 + i * 50, 300, 40, MAROON);
            }
        }

        DrawText("Use mouse to select an option", 260, 530, 18, GRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}