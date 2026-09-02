#include "inventory/Inventory.h"

namespace dungeon
{
    bool Inventory::addConsumable(
        std::unique_ptr<Consumable> consumable)
    {
        if (!consumable)
        {
            return false;
        }

        if (!canAddConsumable(*consumable))
        {
            return false;
        }

        const std::string key =
            std::string(consumable->name());

        m_consumables.push_back(
            std::move(consumable));

        ++m_amounts[key];

        return true;
    }

    bool Inventory::removeConsumable(
        std::size_t index)
    {
        if (index >= m_consumables.size())
        {
            return false;
        }

        const std::string key =
            std::string(
                m_consumables[index]->name());

        m_consumables.erase(
            m_consumables.begin() +
            static_cast<std::ptrdiff_t>(index));

        auto amount = m_amounts.find(key);

        if (amount != m_amounts.end())
        {
            if (amount->second > 0)
            {
                --amount->second;
            }

            if (amount->second == 0)
            {
                m_amounts.erase(amount);
            }
        }

        return true;
    }

    bool Inventory::removeConsumable(
        const Consumable& consumable)
    {
        for (std::size_t i = 0; i < m_consumables.size(); ++i)
        {
            if (m_consumables[i]->name() == consumable.name())
            {
                return removeConsumable(i);
            }
        }

        return false;
    }

    std::size_t Inventory::amount(
        const Consumable& consumable) const noexcept
    {
        const auto amount =
            m_amounts.find(
                std::string(consumable.name()));

        if (amount == m_amounts.end())
        {
            return 0;
        }

        return amount->second;
    }

    bool Inventory::hasConsumable(
        const Consumable& consumable) const noexcept
    {
        return amount(consumable) > 0;
    }

    bool Inventory::canAddConsumable(
        const Consumable& consumable) const noexcept
    {
        return amount(consumable) <
            static_cast<std::size_t>(
                consumable.maximumAmount());
    }

    const std::vector<std::unique_ptr<Consumable>>&
        Inventory::consumables() const noexcept
    {
        return m_consumables;
    }
}