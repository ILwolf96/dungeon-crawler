#include "EnemyFactory.h"

#include "Dragon.h"
#include "Goblin.h"
#include "Skeleton.h"

#include <stdexcept>
#include <string>

namespace dungeon
{
    std::unique_ptr<Enemy> EnemyFactory::create(
        std::string_view type,
        int maxHp,
        int attack,
        int defense,
        int x,
        int y)
    {
        if (type == "goblin")
        {
            return std::make_unique<Goblin>(
                maxHp,
                attack,
                defense,
                x,
                y);
        }

        if (type == "skeleton")
        {
            return std::make_unique<Skeleton>(
                maxHp,
                attack,
                defense,
                x,
                y);
        }

        if (type == "dragon")
        {
            return std::make_unique<Dragon>(
                maxHp,
                attack,
                defense,
                x,
                y);
        }

        throw std::invalid_argument(
            "Unknown enemy type: " + std::string(type));
    }
}