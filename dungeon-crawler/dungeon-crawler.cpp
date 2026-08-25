#include "raylib.h"

int main()
{
    InitWindow(640, 360, "Dungeon Crawler - raylib Test");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText(
            "raylib is working!",
            20,
            20,
            30,
            DARKGRAY);

        DrawRectangle(
            270,
            150,
            100,
            60,
            MAROON);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}