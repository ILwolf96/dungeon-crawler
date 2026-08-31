#pragma once

#include "inventory/Consumable.h"

namespace dungeon
{
    class RagePotion final : public Consumable
    {
    public:
        static constexpr int DamageBonus = 1;
        static constexpr int MaxAmount = 3;

        [[nodiscard]]
        std::string_view name() const noexcept override;

        [[nodiscard]]
        int maximumAmount() const noexcept override;
    };
}