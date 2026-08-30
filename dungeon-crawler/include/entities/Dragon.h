#pragma once

#include "entities/Enemy.h"

namespace dungeon
{
    class Dragon final : public Enemy
    {
    public:
        Dragon(
            int maxHp,
            const CombatStats& stats,
            int x,
            int y);

        [[nodiscard]]
        std::string_view type() const noexcept override;
    };
}