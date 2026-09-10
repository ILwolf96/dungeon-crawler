#include "entities/Chest.h"

#include <algorithm>
#include <stdexcept>

namespace dungeon
{
    Chest::Chest(int x, int y)
        : m_x(x),
        m_y(y)
    {
    }

    int Chest::x() const noexcept
    {
        return m_x;
    }

    int Chest::y() const noexcept
    {
        return m_y;
    }

    void Chest::setPosition(int x, int y)
    {
        m_x = x;
        m_y = y;
    }

    int Chest::currentHp() const noexcept
    {
        return m_currentHp;
    }

    int Chest::maxHp() const noexcept
    {
        return m_maxHp;
    }

    void Chest::takeDamage(int amount)
    {
        if (amount < 0)
        {
            throw std::invalid_argument(
                "Chest damage cannot be negative.");
        }

        m_currentHp =
            std::max(0, m_currentHp - amount);
    }

    void Chest::respawn() noexcept
    {
        m_currentHp = m_maxHp;
    }

    bool Chest::canDefend() const noexcept
    {
        return false;
    }

    bool Chest::isDefeated() const noexcept
    {
        return m_currentHp <= 0;
    }

    const char* Chest::targetType() const noexcept
    {
        return "Chest";
    }

    CombatStats Chest::combatStats() const noexcept
    {
        return m_stats;
    }

    int Chest::tier() const noexcept
    {
        return m_tier;
    }
}