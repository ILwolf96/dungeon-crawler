#include "entities/Dragon.h"

namespace dungeon
{
    Dragon::Dragon(
        int maxHp,
        int tier,
        const CombatStats& stats,
        int x,
        int y)
        : Enemy(maxHp, tier, stats, x, y)
    {
    }

    std::string_view Dragon::type() const noexcept
    {
        return "Dragon";
    }
}