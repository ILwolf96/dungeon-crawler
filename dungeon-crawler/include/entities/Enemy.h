#pragma once

#include "combat/CombatStats.h"
#include "combat/CombatTarget.h"

#include <string_view>

namespace dungeon
{
    class Enemy : public CombatTarget
    {
    public:
        virtual ~Enemy() = default;

        [[nodiscard]]
        virtual std::string_view type() const noexcept = 0;

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

        [[nodiscard]]
        bool isDefeated() const noexcept override;

        [[nodiscard]]
        bool canDefend() const noexcept override;

        [[nodiscard]]
        const char* targetType() const noexcept override;

        [[nodiscard]]
        CombatStats combatStats() const noexcept override;

        [[nodiscard]]
        const CombatStats& stats() const noexcept;

        [[nodiscard]]
        int tier() const noexcept;

    protected:
        Enemy(
            int maxHp,
            int tier,
            const CombatStats& stats,
            int x,
            int y);

    private:
        int m_x;
        int m_y;

        int m_currentHp;
        int m_maxHp;

        int m_tier;

        CombatStats m_stats;
    };
}