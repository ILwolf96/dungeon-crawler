#include "app/Game.h"
#include "rendering/Renderer.h"

#include "raylib.h"

#include <iostream>
#include <exception>

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
        std::cerr
            << "Error: No configuration file was provided.\n"
            << "Usage: dungeon-crawler <config-file>\n";

        return 1;
    }

    dungeon::Game game;
    dungeon::Renderer renderer;

    try
    {
        game.load(argv[1]);
    }
    catch (const std::exception& error)
    {
        std::cerr
            << "Error: Failed to load configuration.\n"
            << error.what()
            << '\n';

        return 1;
    }

    InitWindow(
        game.windowWidth(),
        game.windowHeight(),
        game.title().c_str());

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