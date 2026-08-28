#pragma once

#include "Enemy.h"

namespace dungeon
{
    class Goblin final : public Enemy
    {
    public:
        Goblin(
            int maxHp,
            int attack,
            int defense,
            int x,
            int y);

        [[nodiscard]]
        std::string_view type() const noexcept override;
    };
}