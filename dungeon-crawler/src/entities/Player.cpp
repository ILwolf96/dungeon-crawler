#include "entities/Player.h"

#include <algorithm>
#include <stdexcept>

namespace dungeon
{
    Player::Player(int x, int y)
        : m_x(x),
        m_y(y)
    {
    }

    int Player::x() const noexcept
    {
        return m_x;
    }

    int Player::y() const noexcept
    {
        return m_y;
    }

    void Player::setPosition(int x, int y)
    {
        m_x = x;
        m_y = y;
    }

    int Player::currentHp() const noexcept
    {
        return m_currentHp;
    }

    int Player::maxHp() const noexcept
    {
        return m_maxHp;
    }

    void Player::takeDamage(int amount)
    {
        if (amount < 0)
        {
            throw std::invalid_argument(
                "Player damage cannot be negative.");
        }

        m_currentHp = std::max(0, m_currentHp - amount);
    }

    bool Player::isDefeated() const noexcept
    {
        return m_currentHp <= 0;
    }

    const char* Player::targetType() const noexcept
    {
        return "Player";
    }

    const CombatStats& Player::stats() const noexcept
    {
        return m_stats;
    }
}