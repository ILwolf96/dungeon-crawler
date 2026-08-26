#include "Game.h"
#include "Renderer.h"

#include "raylib.h"

int main(int argc, char** argv)
{
    dungeon::Game game;
    dungeon::Renderer renderer;

    if (argc < 2)
    {
        return 1;
    }

    game.load(argv[1]);

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