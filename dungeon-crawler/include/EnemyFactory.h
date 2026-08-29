#pragma once

#include "CombatStats.h"
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
            const CombatStats& stats,
            int x,
            int y);
    };
}