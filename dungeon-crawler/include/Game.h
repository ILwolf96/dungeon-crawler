#pragma once

#include "Map.h"
#include "Player.h"

#include <string_view>

namespace dungeon
{
    enum class Action
    {
        None,
        MoveUp,
        MoveDown,
        MoveLeft,
        MoveRight
    };

    class Game
    {
    public:
        Game();

        void load(std::string_view filePath);
        void handleAction(Action action);

        [[nodiscard]]
        const Map& map() const noexcept;

        [[nodiscard]]
        const Player& player() const noexcept;

    private:
        Map m_map;
        Player m_player;
    };
}