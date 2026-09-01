#include "entities/Enemy.h"
#include <algorithm>
#include <stdexcept>

namespace dungeon
{
    Enemy::Enemy(
        int maxHp,
        int tier,
        const CombatStats& stats,
        int x,
        int y)
        : m_x(x),
        m_y(y),
        m_currentHp(maxHp),
        m_maxHp(maxHp),
        m_tier(tier),
        m_stats(stats)
    {
        if (maxHp <= 0)
        {
            throw std::invalid_argument(
                "Enemy max HP must be greater than zero.");
        }

        if (tier <= 0)
        {
            throw std::invalid_argument(
                "Enemy tier must be greater than zero.");
        }

        if (stats.attacks <= 0)
        {
            throw std::invalid_argument(
                "Enemy attacks must be greater than zero.");
        }

        if (stats.precision < 1 || stats.precision > 6)
        {
            throw std::invalid_argument(
                "Enemy precision must be between 1 and 6.");
        }

        if (stats.strength < 0)
        {
            throw std::invalid_argument(
                "Enemy strength cannot be negative.");
        }

        if (stats.toughness < 0)
        {
            throw std::invalid_argument(
                "Enemy toughness cannot be negative.");
        }

        if (stats.defense < 1 || stats.defense > 6)
        {
            throw std::invalid_argument(
                "Enemy defense must be between 1 and 6.");
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

    void Enemy::setPosition(int x, int y)
    {
        m_x = x;
        m_y = y;
    }

    int Enemy::currentHp() const noexcept
    {
        return m_currentHp;
    }

    int Enemy::maxHp() const noexcept
    {
        return m_maxHp;
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

    bool Enemy::isDefeated() const noexcept
    {
        return m_currentHp <= 0;
    }

    const char* Enemy::targetType() const noexcept
    {
        return "Enemy";
    }

    int Enemy::tier() const noexcept
    {
        return m_tier;
    }

    const CombatStats& Enemy::stats() const noexcept
    {
        return m_stats;
    }

    CombatStats Enemy::combatStats() const noexcept
    {
        return m_stats;
    }
}