#pragma once

#include <string>

namespace dungeon
{
    enum class LootType
    {
        Weapon,
        Armor,
        Accessory
    };

    struct LootReward
    {
        LootType type;
        std::string id;
        int tier;
    };
}