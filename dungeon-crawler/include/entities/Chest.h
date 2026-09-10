#pragma once

#include "combat/CombatStats.h"
#include "combat/CombatTarget.h"

namespace dungeon
{
    class Chest final : public CombatTarget
    {
    public:
        explicit Chest(int x, int y);

        [[nodiscard]]
        int x() const noexcept override;

        [[nodiscard]]
        int y() const noexcept override;

        void setPosition(int x, int y) override;

        [[nodiscard]]
        int currentHp() const noexcept override;

        [[nodiscard]]
        int maxHp() const noexcept override;

        void takeDamage(int amount) override;

        void respawn() noexcept;
        
        bool canDefend() const noexcept override;

        [[nodiscard]]
        bool isDefeated() const noexcept override;

        [[nodiscard]]
        const char* targetType() const noexcept override;

        [[nodiscard]]
        CombatStats combatStats() const noexcept override;

        [[nodiscard]]
        int tier() const noexcept;

    private:
        int m_x;
        int m_y;

        int m_currentHp{ 1 };
        int m_maxHp{ 1 };

        CombatStats m_stats{
            0, // attacks
            0, // precision
            0, // strength
            0, // toughness
            0  // defense
        };

        int m_tier{ 1 };
    };
}