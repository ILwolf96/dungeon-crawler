#pragma once

namespace dungeon
{
    class Game;

    class Renderer
    {
    public:
        Renderer() = default;

        void draw(const Game& game) const;
    };
}