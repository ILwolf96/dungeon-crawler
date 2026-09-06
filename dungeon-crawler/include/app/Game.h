#pragma once

#include "combat/CombatStats.h"
#include "combat/CombatPresentation.h"
#include "entities/Chest.h"
#include "entities/Enemy.h"
#include "entities/Player.h"
#include "world/Map.h"
#include "combat/Combat.h"
#include "combat/RandomDice.h"
#include "config/ConfigData.h"

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
        MoveRight,
        Attack,
        Inventory,
        Escape
    };

    class Game
    {
    public:
        Game();

        void load(std::string_view filePath);
        void handleAction(Action action);
        void update(float deltaSeconds);

        bool inCombat() const noexcept;

        Combat* combat() noexcept;
        const Combat* combat() const noexcept;

        void startCombat(CombatTarget& target);

        int displayedPlayerHp() const noexcept;
        int displayedCombatTargetHp() const noexcept;


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

        [[nodiscard]]
        bool combatPresentationActive() const noexcept;

        [[nodiscard]]
        const CombatPresentation& combatPresentation() const noexcept;

    private:
        Map m_map;
        Player m_player;

        CombatTarget* combatTargetAt(int x, int y) noexcept;
        void finishCombatIfNeeded();
        void performCombatAttack();
        void performCombatEscape();
        void openCombatInventory();
        void closeCombatInventory();
        void advanceCombatPresentation();

        std::vector<std::unique_ptr<Enemy>> m_enemies;
        std::vector<std::unique_ptr<Chest>> m_chests;

        std::unique_ptr<Combat> m_combat;
        CombatPresentation m_combatPresentation;

        int m_displayedPlayerHp{ 0 };
        int m_displayedCombatTargetHp{ 0 };
        bool m_pendingPlayerHpSync{ false };


        RandomDice m_combatDice;
        bool m_inventoryOpen{ false };
        bool m_pendingEnemyTurn{ false };
        bool m_pendingCombatFinish{ false };

        bool useHealthPotion();
        bool useRagePotion();

        config::ConfigData m_configData;

        int m_windowWidth{ 640 };
        int m_windowHeight{ 360 };
        std::string m_title{ "Dungeon Crawler" };
    };
}