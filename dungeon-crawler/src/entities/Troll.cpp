#include "entities/Troll.h"

namespace dungeon
{
    Troll::Troll(
        int maxHp,
        int tier,
        const CombatStats& stats,
        int x,
        int y)
        : Enemy(maxHp, tier, stats, x, y)
    {
    }

    std::string_view Troll::type() const noexcept
    {
        return "Troll";
    }
}