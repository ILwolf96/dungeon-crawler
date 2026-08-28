#pragma once

#include "Enemy.h"

#include <memory>
#include <string_view>

namespace dungeon
{
    class EnemyFactory
    {
    public:
        [[nodiscard]]
        static std::unique_ptr<Enemy> create(
            std::string_view type,
            int maxHp,
            int attack,
            int defense,
            int x,
            int y);
    };
}