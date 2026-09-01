#pragma once

#include "loot/LootReward.h"

#include <vector>

namespace dungeon
{
    class LootTable
    {
    public:
        [[nodiscard]]
        static std::vector<LootReward>
            rewardsForTier(int tier);
    };
}