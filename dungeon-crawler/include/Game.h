#pragma once

#include "Map.h"
#include "Player.h"

namespace dungeon
{
    class Game
    {
    public:
        Game();

        void update();

        [[nodiscard]]
        const Map& map() const noexcept;

        [[nodiscard]]
        const Player& player() const noexcept;

    private:
        Map m_map;
        Player m_player;
    };
}