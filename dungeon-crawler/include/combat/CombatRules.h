#pragma once

namespace dungeon
{
    class CombatRules
    {
    public:
        [[nodiscard]]
        static bool passesRoll(
            int roll,
            int target) noexcept;

        [[nodiscard]]
        static int woundTarget(
            int strength,
            int toughness) noexcept;

        [[nodiscard]]
        static bool isDefeated(
            int currentHp) noexcept;
    };
}