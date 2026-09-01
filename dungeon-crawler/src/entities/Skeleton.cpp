#include "entities/Skeleton.h"

namespace dungeon
{
    Skeleton::Skeleton(
        int maxHp,
        int tier,
        const CombatStats& stats,
        int x,
        int y)
        : Enemy(maxHp, tier, stats, x, y)
    {
    }

    std::string_view Skeleton::type() const noexcept
    {
        return "Skeleton";
    }
}