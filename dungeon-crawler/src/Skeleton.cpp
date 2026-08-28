#include "Skeleton.h"

namespace dungeon
{
    Skeleton::Skeleton(
        int maxHp,
        int attack,
        int defense,
        int x,
        int y)
        : Enemy(maxHp, attack, defense, x, y)
    {
    }

    std::string_view Skeleton::type() const noexcept
    {
        return "Skeleton";
    }
}