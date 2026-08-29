#include "EnemyFactory.h"

#include "Dragon.h"
#include "Goblin.h"
#include "Orc.h"
#include "Skeleton.h"
#include "Troll.h"

#include <stdexcept>
#include <string>

namespace dungeon
{
    std::unique_ptr<Enemy> EnemyFactory::create(
        std::string_view type,
        int maxHp,
        const CombatStats& stats,
        int x,
        int y)
    {
        if (type == "goblin")
        {
            return std::make_unique<Goblin>(
                maxHp,
                stats,
                x,
                y);
        }

        if (type == "skeleton")
        {
            return std::make_unique<Skeleton>(
                maxHp,
                stats,
                x,
                y);
        }

        if (type == "orc")
        {
            return std::make_unique<Orc>(
                maxHp,
                stats,
                x,
                y);
        }

        if (type == "troll")
        {
            return std::make_unique<Troll>(
                maxHp,
                stats,
                x,
                y);
        }

        if (type == "dragon")
        {
            return std::make_unique<Dragon>(
                maxHp,
                stats,
                x,
                y);
        }

        throw std::invalid_argument(
            "Unknown enemy type: " + std::string(type));
    }
}