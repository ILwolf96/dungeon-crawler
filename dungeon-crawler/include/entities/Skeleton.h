#pragma once

#include "entities/Enemy.h"

namespace dungeon
{
    class Skeleton final : public Enemy
    {
    public:
        Skeleton(
            int maxHp,
            int tier,
            const CombatStats& stats,
            int x,
            int y);

        [[nodiscard]]
        std::string_view type() const noexcept override;
    };
}