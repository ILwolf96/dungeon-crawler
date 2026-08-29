#include "Dragon.h"

namespace dungeon
{
    Dragon::Dragon(
        int maxHp,
        const CombatStats& stats,
        int x,
        int y)
        : Enemy(maxHp, stats, x, y)
    {
    }

    std::string_view Dragon::type() const noexcept
    {
        return "Dragon";
    }
}