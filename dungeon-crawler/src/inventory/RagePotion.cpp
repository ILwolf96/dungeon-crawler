#include "inventory/RagePotion.h"

namespace dungeon
{
    std::string_view RagePotion::name() const noexcept
    {
        return "Rage Potion";
    }

    int RagePotion::maximumAmount() const noexcept
    {
        return MaxAmount;
    }
}