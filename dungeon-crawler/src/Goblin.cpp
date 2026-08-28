#include "Goblin.h"

namespace dungeon
{
    Goblin::Goblin(
        int maxHp,
        int attack,
        int defense,
        int x,
        int y)
        : Enemy(maxHp, attack, defense, x, y)
    {
    }

    std::string_view Goblin::type() const noexcept
    {
        return "Goblin";
    }
}