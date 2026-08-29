#include "Orc.h"

namespace dungeon
{
    Orc::Orc(
        int maxHp,
        const CombatStats& stats,
        int x,
        int y)
        : Enemy(maxHp, stats, x, y)
    {
    }

    std::string_view Orc::type() const noexcept
    {
        return "Orc";
    }
}