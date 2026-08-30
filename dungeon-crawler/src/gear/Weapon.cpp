#include "gear/Weapon.h"

#include <stdexcept>
#include <utility>

namespace dungeon
{
    Weapon::Weapon(
        std::string name,
        int tier,
        int strength,
        int damage)
        : m_name(std::move(name)),
        m_tier(tier),
        m_strength(strength),
        m_damage(damage)
    {
        if (m_name.empty())
        {
            throw std::invalid_argument(
                "Weapon name cannot be empty.");
        }

        if (tier < 0)
        {
            throw std::invalid_argument(
                "Weapon tier cannot be negative.");
        }

        if (strength < 0)
        {
            throw std::invalid_argument(
                "Weapon strength cannot be negative.");
        }

        if (damage <= 0)
        {
            throw std::invalid_argument(
                "Weapon damage must be greater than zero.");
        }
    }

    const std::string& Weapon::name() const noexcept
    {
        return m_name;
    }

    int Weapon::tier() const noexcept
    {
        return m_tier;
    }

    int Weapon::strength() const noexcept
    {
        return m_strength;
    }

    int Weapon::damage() const noexcept
    {
        return m_damage;
    }
}