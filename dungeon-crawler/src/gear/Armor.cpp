#include "gear/Armor.h"

#include <stdexcept>
#include <utility>

namespace dungeon
{
    Armor::Armor(
        std::string name,
        int tier,
        int defense)
        : m_name(std::move(name)),
        m_tier(tier),
        m_defense(defense)
    {
        if (m_name.empty())
        {
            throw std::invalid_argument(
                "Armor name cannot be empty.");
        }

        if (tier < 0)
        {
            throw std::invalid_argument(
                "Armor tier cannot be negative.");
        }

        if (defense < 0)
        {
            throw std::invalid_argument(
                "Armor defense cannot be negative.");
        }
    }

    const std::string& Armor::name() const noexcept
    {
        return m_name;
    }

    int Armor::tier() const noexcept
    {
        return m_tier;
    }

    int Armor::defense() const noexcept
    {
        return m_defense;
    }
}