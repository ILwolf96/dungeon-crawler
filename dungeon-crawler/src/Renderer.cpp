#include "Renderer.h"

#include "Game.h"

#include "raylib.h"

namespace dungeon
{
    void Renderer::draw(const Game& /*game*/) const
    {
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
    }
}