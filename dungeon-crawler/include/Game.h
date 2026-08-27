#pragma once

#include "Map.h"
#include "Player.h"

#include <string>
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
        int windowWidth() const noexcept;

        [[nodiscard]]
        int windowHeight() const noexcept;

        [[nodiscard]]
        const std::string& title() const noexcept;

        [[nodiscard]]
        const Map& map() const noexcept;

        [[nodiscard]]
        const Player& player() const noexcept;

    private:
        Map m_map;
        Player m_player;

        int m_windowWidth{ 640 };
        int m_windowHeight{ 360 };
        std::string m_title{ "Dungeon Crawler" };
    };
}