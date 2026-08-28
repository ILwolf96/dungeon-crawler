#include "Dragon.h"

namespace dungeon
{
    Dragon::Dragon(
        int maxHp,
        int attack,
        int defense,
        int x,
        int y)
        : Enemy(maxHp, attack, defense, x, y)
    {
    }

    std::string_view Dragon::type() const noexcept
    {
        return "Dragon";
    }
}