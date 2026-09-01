#pragma once

#include "config/ConfigData.h"
#include "loot/LootReward.h"

#include <string>

namespace dungeon
{
    class IDice;
    class Player;

    class LootGenerator
    {
    public:
        [[nodiscard]]
        static bool award(
            Player& player,
            int tier,
            IDice& dice,
            const config::ConfigData& data,
            std::string& message);
    };
}