#pragma once

#include "combat/CombatStats.h"

namespace dungeon
{
    class CombatTarget
    {
    public:
        virtual ~CombatTarget() = default;

        [[nodiscard]]
        virtual int x() const noexcept = 0;

        [[nodiscard]]
        virtual int y() const noexcept = 0;

        virtual void setPosition(int x, int y) = 0;

        [[nodiscard]]
        virtual int currentHp() const noexcept = 0;

        [[nodiscard]]
        virtual int maxHp() const noexcept = 0;

        virtual void takeDamage(int amount) = 0;

        [[nodiscard]]
        virtual bool isDefeated() const noexcept = 0;

        [[nodiscard]]
        virtual const char* targetType() const noexcept = 0;

        [[nodiscard]]
        virtual CombatStats combatStats() const noexcept = 0;
    };
}