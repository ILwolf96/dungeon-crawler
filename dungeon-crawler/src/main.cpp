#include "Game.h"
#include "Renderer.h"

#include "raylib.h"

namespace
{
    dungeon::Action pollAction()
    {
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
        {
            return dungeon::Action::MoveUp;
        }

        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
        {
            return dungeon::Action::MoveDown;
        }

        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
        {
            return dungeon::Action::MoveLeft;
        }

        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
        {
            return dungeon::Action::MoveRight;
        }

        return dungeon::Action::None;
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        return 1;
    }

    dungeon::Game game;
    dungeon::Renderer renderer;

    game.load(argv[1]);

    InitWindow(
        640,
        360,
        "Dungeon Crawler");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const dungeon::Action action = pollAction();

        game.handleAction(action);

        BeginDrawing();

        renderer.draw(game);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}