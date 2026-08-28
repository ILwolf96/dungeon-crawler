#pragma once

#include "Enemy.h"

namespace dungeon
{
    class Dragon final : public Enemy
    {
    public:
        Dragon(
            int maxHp,
            int attack,
            int defense,
            int x,
            int y);

        [[nodiscard]]
        std::string_view type() const noexcept override;
    };
}