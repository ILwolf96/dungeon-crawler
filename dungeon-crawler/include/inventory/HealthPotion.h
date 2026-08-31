#pragma once

#include "inventory/Consumable.h"

namespace dungeon
{
    class HealthPotion final : public Consumable
    {
    public:
        static constexpr int HealAmount = 3;
        static constexpr int MaxAmount = 3;

        [[nodiscard]]
        std::string_view name() const noexcept override;

        [[nodiscard]]
        int maximumAmount() const noexcept override;
    };
}