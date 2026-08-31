#include "inventory/HealthPotion.h"

namespace dungeon
{
    std::string_view HealthPotion::name() const noexcept
    {
        return "Health Potion";
    }

    int HealthPotion::maximumAmount() const noexcept
    {
        return MaxAmount;
    }
}