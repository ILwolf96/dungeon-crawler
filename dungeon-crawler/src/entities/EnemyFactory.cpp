#include "entities/EnemyFactory.h"

#include "entities/Goblin.h"
#include "entities/Skeleton.h"
#include "entities/Orc.h"
#include "entities/Troll.h"

#include "entities/Dragon.h"
#include <stdexcept>
#include <string>

namespace dungeon
{
    std::unique_ptr<Enemy> EnemyFactory::create(
        std::string_view type,
        int maxHp,
        int tier,
        const CombatStats& stats,
        int x,
        int y)
    {
        if (type == "goblin")
        {
            return std::make_unique<Goblin>(
                maxHp,
                tier,
                stats,
                x,
                y);
        }

        if (type == "skeleton")
        {
            return std::make_unique<Skeleton>(
                maxHp,
                tier,
                stats,
                x,
                y);
        }

        if (type == "orc")
        {
            return std::make_unique<Orc>(
                maxHp,
                tier,
                stats,
                x,
                y);
        }

        if (type == "troll")
        {
            return std::make_unique<Troll>(
                maxHp,
                tier,
                stats,
                x,
                y);
        }

        if (type == "dragon")
        {
            return std::make_unique<Dragon>(
                maxHp,
                tier,
                stats,
                x,
                y);
        }

        throw std::invalid_argument(
            "Unknown enemy type: " + std::string(type));
    }
}