#pragma once

#include "entities/Enemy.h"

namespace dungeon
{
    class Goblin final : public Enemy
    {
    public:
        Goblin(
            int maxHp,
            int tier,
            const CombatStats& stats,
            int x,
            int y);

        [[nodiscard]]
        std::string_view type() const noexcept override;
    };
}