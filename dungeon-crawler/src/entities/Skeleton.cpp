#include "entities/Skeleton.h"

namespace dungeon
{
    Skeleton::Skeleton(
        int maxHp,
        const CombatStats& stats,
        int x,
        int y)
        : Enemy(maxHp, stats, x, y)
    {
    }

    std::string_view Skeleton::type() const noexcept
    {
        return "Skeleton";
    }
}