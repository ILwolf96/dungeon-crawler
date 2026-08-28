#include "Enemy.h"

#include <algorithm>
#include <stdexcept>

namespace dungeon
{
    Enemy::Enemy(
        int maxHp,
        int attack,
        int defense,
        int x,
        int y)
        : m_x(x),
        m_y(y),
        m_currentHp(maxHp),
        m_maxHp(maxHp),
        m_attack(attack),
        m_defense(defense)
    {
        if (maxHp <= 0)
        {
            throw std::invalid_argument(
                "Enemy max HP must be greater than zero.");
        }

        if (attack < 0)
        {
            throw std::invalid_argument(
                "Enemy attack cannot be negative.");
        }

        if (defense < 0)
        {
            throw std::invalid_argument(
                "Enemy defense cannot be negative.");
        }
    }

    int Enemy::x() const noexcept
    {
        return m_x;
    }

    int Enemy::y() const noexcept
    {
        return m_y;
    }

    int Enemy::currentHp() const noexcept
    {
        return m_currentHp;
    }

    int Enemy::maxHp() const noexcept
    {
        return m_maxHp;
    }

    int Enemy::attack() const noexcept
    {
        return m_attack;
    }

    int Enemy::defense() const noexcept
    {
        return m_defense;
    }

    void Enemy::setPosition(int x, int y)
    {
        m_x = x;
        m_y = y;
    }

    void Enemy::takeDamage(int amount)
    {
        if (amount < 0)
        {
            throw std::invalid_argument(
                "Enemy damage cannot be negative.");
        }

        m_currentHp = std::max(0, m_currentHp - amount);
    }
}