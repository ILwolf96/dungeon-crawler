#pragma once

#include "inventory/Consumable.h"

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

namespace dungeon
{
    class Inventory
    {
    public:
        Inventory() = default;

        bool addConsumable(
            std::unique_ptr<Consumable> consumable);

        bool removeConsumable(
            std::size_t index);

        [[nodiscard]]
        bool canAddConsumable(
            const Consumable& consumable) const noexcept;

        [[nodiscard]]
        std::size_t amount(
            const Consumable& consumable) const noexcept;

        [[nodiscard]]
        bool hasConsumable(
            const Consumable& consumable) const noexcept;

        [[nodiscard]]
        const std::vector<std::unique_ptr<Consumable>>&
            consumables() const noexcept;

    private:
        std::vector<std::unique_ptr<Consumable>>
            m_consumables;

        std::unordered_map<std::string, std::size_t>
            m_amounts;
    };
}