#pragma once

namespace dungeon
{
    class Game;

    class Renderer
    {
    public:
        Renderer() = default;

        void draw(const Game& game) const;

    private:
        static constexpr int TileSize = 48;
        static constexpr int MapOffsetX = 24;
        static constexpr int MapOffsetY = 24;
    };
}