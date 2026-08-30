#include "Accessory.h"

#include <stdexcept>
#include <utility>

namespace dungeon
{
    Accessory::Accessory(
        std::string name,
        int tier,
        int attacksBonus,
        int precisionBonus,
        int maxHpBonus)
        : m_name(std::move(name)),
        m_tier(tier),
        m_attacksBonus(attacksBonus),
        m_precisionBonus(precisionBonus),
        m_maxHpBonus(maxHpBonus)
    {
        if (m_name.empty())
        {
            throw std::invalid_argument(
                "Accessory name cannot be empty.");
        }

        if (tier < 0)
        {
            throw std::invalid_argument(
                "Accessory tier cannot be negative.");
        }
    }

    const std::string& Accessory::name() const noexcept
    {
        return m_name;
    }

    int Accessory::tier() const noexcept
    {
        return m_tier;
    }

    int Accessory::attacksBonus() const noexcept
    {
        return m_attacksBonus;
    }

    int Accessory::precisionBonus() const noexcept
    {
        return m_precisionBonus;
    }

    int Accessory::maxHpBonus() const noexcept
    {
        return m_maxHpBonus;
    }
}