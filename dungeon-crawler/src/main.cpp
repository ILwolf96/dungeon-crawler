#include "Game.h"
#include "Renderer.h"

#include "raylib.h"

int main()
{
    dungeon::Game game;
    dungeon::Renderer renderer;

    InitWindow(
        640,
        360,
        "Dungeon Crawler");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        game.update();

        BeginDrawing();

        renderer.draw(game);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}