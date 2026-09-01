#include "entities/Orc.h"

namespace dungeon
{
    Orc::Orc(
        int maxHp,
        int tier,
        const CombatStats& stats,
        int x,
        int y)
        : Enemy(maxHp, tier, stats, x, y)
    {
    }

    std::string_view Orc::type() const noexcept
    {
        return "Orc";
    }
}