#include "combat/CombatRules.h"

namespace dungeon
{
    bool CombatRules::passesRoll(
        int roll,
        int target) noexcept
    {
        return roll >= target;
    }

    int CombatRules::woundTarget(
        int strength,
        int toughness) noexcept
    {
        if (toughness <= 0)
        {
            return 2;
        }

        if (strength >= toughness * 2)
        {
            return 2;
        }

        if (strength > toughness)
        {
            return 3;
        }

        if (strength == toughness)
        {
            return 4;
        }

        if (strength * 2 <= toughness)
        {
            return 6;
        }

        return 5;
    }

    bool CombatRules::isDefeated(
        int currentHp) noexcept
    {
        return currentHp <= 0;
    }
}