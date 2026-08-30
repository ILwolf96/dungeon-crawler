#pragma once

#include "entities/Enemy.h"

namespace dungeon
{
    class Troll final : public Enemy
    {
    public:
        Troll(
            int maxHp,
            const CombatStats& stats,
            int x,
            int y);

        [[nodiscard]]
        std::string_view type() const noexcept override;
    };
}