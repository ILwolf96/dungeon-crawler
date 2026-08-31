#pragma once

#include <string_view>

namespace dungeon
{
    class Consumable
    {
    public:
        virtual ~Consumable() = default;

        [[nodiscard]]
        virtual std::string_view name() const noexcept = 0;

        [[nodiscard]]
        virtual int maximumAmount() const noexcept = 0;
    };
}