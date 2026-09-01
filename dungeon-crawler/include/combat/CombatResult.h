#pragma once

#include <string>

namespace dungeon
{
    enum class AttackResolution
    {
        Missed,
        FailedToWound,
        Defended,
        Damaged,
        Defeated
    };

    struct AttackResult
    {
        int precisionRoll{ 0 };
        int precisionTarget{ 0 };

        int woundRoll{ 0 };
        int woundTarget{ 0 };

        int defenseRoll{ 0 };
        int defenseTarget{ 0 };

        int damage{ 0 };

        AttackResolution resolution{
            AttackResolution::Missed
        };
    };

    struct CombatResult
    {
        bool targetDefeated{ false };
        bool attackerDefeated{ false };

        std::string message;
    };
}