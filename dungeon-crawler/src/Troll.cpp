#include "Troll.h"

namespace dungeon
{
    Troll::Troll(
        int maxHp,
        const CombatStats& stats,
        int x,
        int y)
        : Enemy(maxHp, stats, x, y)
    {
    }

    std::string_view Troll::type() const noexcept
    {
        return "Troll";
    }
}