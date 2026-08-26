#pragma once

#include "Map.h"
#include "Player.h"

#include <string_view>

namespace dungeon
{
    class Game
    {
    public:
        Game();

        void load(std::string_view filePath);
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