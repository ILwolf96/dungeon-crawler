#include "entities/Goblin.h"

namespace dungeon
{
    Goblin::Goblin(
        int maxHp,
        const CombatStats& stats,
        int x,
        int y)
        : Enemy(maxHp, stats, x, y)
    {
    }

    std::string_view Goblin::type() const noexcept
    {
        return "Goblin";
    }
}