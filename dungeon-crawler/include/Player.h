#pragma once

#include "CombatStats.h"
#include "CombatTarget.h"

namespace dungeon
{
    class Player final : public CombatTarget
    {
    public:
        Player() = default;
        Player(int x, int y);

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

        [[nodiscard]]
        bool isDefeated() const noexcept override;

        [[nodiscard]]
        const char* targetType() const noexcept override;

        [[nodiscard]]
        const CombatStats& stats() const noexcept;

    private:
        int m_x{ 0 };
        int m_y{ 0 };

        int m_currentHp{ 6 };
        int m_maxHp{ 6 };

        CombatStats m_stats{
            2, // attacks
            3, // precision
            3, // strength - temporary until equipment is implemented
            4, // toughness
            5  // defense - temporary until armor is implemented
        };
    };
}