#pragma once

#include "combat/CombatResult.h"

#include <string>
#include <vector>

namespace dungeon
{
    class CombatTarget;
    class IDice;
    class Player;

    class Combat
    {
    public:
        Combat(
            Player& player,
            CombatTarget& target,
            IDice& dice);

        [[nodiscard]]
        CombatResult playerAttack();

        [[nodiscard]]
        CombatResult enemyTurn();

        [[nodiscard]]
        CombatResult escape();

        [[nodiscard]]
        bool isActive() const noexcept;

        [[nodiscard]]
        bool playerWon() const noexcept;

        [[nodiscard]]
        bool playerLost() const noexcept;

        [[nodiscard]]
        bool escaped() const noexcept;

        [[nodiscard]]
        CombatTarget& target() noexcept;

        [[nodiscard]]
        const CombatTarget& target() const noexcept;

        [[nodiscard]]
        const std::vector<AttackResult>&
            lastPlayerAttacks() const noexcept;

        [[nodiscard]]
        const std::vector<AttackResult>&
            lastEnemyAttacks() const noexcept;

    private:
        AttackResult resolveAttack(
            CombatTarget& attacker,
            CombatTarget& defender,
            int damage);

        CombatResult resolveAttacks(
            CombatTarget& attacker,
            CombatTarget& defender,
            int damage,
            std::vector<AttackResult>& results);

        Player& m_player;
        CombatTarget& m_target;
        IDice& m_dice;

        std::vector<AttackResult> m_lastPlayerAttacks;
        std::vector<AttackResult> m_lastEnemyAttacks;

        bool m_active{ true };
        bool m_playerEscaped{ false };
    };
}