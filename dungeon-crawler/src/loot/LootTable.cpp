#include "loot/LootTable.h"

namespace dungeon
{
    std::vector<LootReward>
        LootTable::rewardsForTier(int tier)
    {
        switch (tier)
        {
        case 2:
            return {
                { LootType::Weapon, "sword", 2 },
                { LootType::Armor, "chainmail", 2 }
            };

        case 3:
            return {
                { LootType::Weapon, "greatsword", 3 },
                { LootType::Armor, "plate", 3 },
                { LootType::Accessory, "magic_skull", 3 }
            };

        case 4:
            return {
                { LootType::Weapon, "orcish_axe", 4 },
                { LootType::Armor, "orcish_plate", 4 },
                { LootType::Accessory, "orc_fang", 4 }
            };

        case 5:
            return {
                { LootType::Weapon, "troll_arm", 5 },
                { LootType::Armor, "troll_hide", 5 },
                { LootType::Accessory, "troll_heart", 5 }
            };

        case 6:
            return {
                { LootType::Weapon, "dragonbone_blade", 6 },
                { LootType::Armor, "dragonscale_plate", 6 }
            };

        default:
            return {};
        }
    }
}