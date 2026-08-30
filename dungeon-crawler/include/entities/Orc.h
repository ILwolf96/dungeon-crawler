#pragma once

#include "entities/Enemy.h"

namespace dungeon
{
    class Orc final : public Enemy
    {
    public:
        Orc(
            int maxHp,
            const CombatStats& stats,
            int x,
            int y);

        [[nodiscard]]
        std::string_view type() const noexcept override;
    };
}