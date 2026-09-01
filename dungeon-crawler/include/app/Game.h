#pragma once

#include "combat/CombatStats.h"
#include "entities/Chest.h"
#include "entities/Enemy.h"
#include "entities/Player.h"
#include "world/Map.h"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

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
        Player& player() noexcept;

        [[nodiscard]]
        const Player& player() const noexcept;

        [[nodiscard]]
        const std::vector<std::unique_ptr<Enemy>>& enemies() const noexcept;

        [[nodiscard]]
        const std::vector<std::unique_ptr<Chest>>& chests() const noexcept;

    private:
        Map m_map;
        Player m_player;

        std::vector<std::unique_ptr<Enemy>> m_enemies;
        std::vector<std::unique_ptr<Chest>> m_chests;

        int m_windowWidth{ 640 };
        int m_windowHeight{ 360 };
        std::string m_title{ "Dungeon Crawler" };
    };
}