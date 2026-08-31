#include "entities/Player.h"

#include <algorithm>
#include <stdexcept>

namespace dungeon
{
    Player::Player()
        : Player(0, 0)
    {
    }

    Player::Player(int x, int y)
        : m_x(x),
        m_y(y),
        m_currentHp(m_baseMaxHp),
        m_baseStats{
            2,
            3,
            1,
            4,
            0
        },
        m_equipment(),
        m_inventory()
    {
        /*
        m_equipment.equipWeapon(
            std::make_unique<Weapon>(
                "Club",
                1,
                3,
                1));

        m_equipment.equipArmor(
            std::make_unique<Armor>(
                "Gambeson",
                1,
                5));
        */
    }

    void Player::initialize(
        int maxHp,
        const CombatStats& baseStats,
        std::unique_ptr<Weapon> weapon,
        std::unique_ptr<Armor> armor)
    {
        if (maxHp <= 0)
        {
            throw std::invalid_argument(
                "Player max HP must be greater than zero.");
        }

        if (baseStats.attacks <= 0)
        {
            throw std::invalid_argument(
                "Player attacks must be greater than zero.");
        }

        if (baseStats.precision < 1 ||
            baseStats.precision > 6)
        {
            throw std::invalid_argument(
                "Player precision must be between 1 and 6.");
        }

        if (baseStats.strength < 0)
        {
            throw std::invalid_argument(
                "Player strength cannot be negative.");
        }

        if (baseStats.toughness < 0)
        {
            throw std::invalid_argument(
                "Player toughness cannot be negative.");
        }

        if (baseStats.defense < 0)
        {
            throw std::invalid_argument(
                "Player defense cannot be negative.");
        }

        if (!weapon)
        {
            throw std::invalid_argument(
                "Player must have a weapon.");
        }

        if (!armor)
        {
            throw std::invalid_argument(
                "Player must have armor.");
        }

        m_baseMaxHp = maxHp;
        m_currentHp = maxHp;
        m_baseStats = baseStats;

        m_equipment = Equipment();
        m_inventory = Inventory();

        m_equipment.equipWeapon(
            std::move(weapon));

        m_equipment.equipArmor(
            std::move(armor));
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
        int value = m_baseMaxHp;

        for (const auto& accessory :
            m_equipment.accessories())
        {
            value += accessory->maxHpBonus();
        }

        return value;
    }

    void Player::takeDamage(int amount)
    {
        if (amount < 0)
        {
            throw std::invalid_argument(
                "Player damage cannot be negative.");
        }

        m_currentHp =
            std::max(0, m_currentHp - amount);
    }

    void Player::heal(int amount)
    {
        if (amount < 0)
        {
            throw std::invalid_argument(
                "Player healing cannot be negative.");
        }

        m_currentHp =
            std::min(maxHp(), m_currentHp + amount);
    }

    bool Player::isDefeated() const noexcept
    {
        return m_currentHp <= 0;
    }

    const char* Player::targetType() const noexcept
    {
        return "Player";
    }

    const CombatStats& Player::baseStats() const noexcept
    {
        return m_baseStats;
    }

    CombatStats Player::effectiveStats() const noexcept
    {
        CombatStats result = m_baseStats;

        if (const Weapon* weapon =
            m_equipment.weapon())
        {
            result.strength = weapon->strength();
        }

        if (const Armor* armor =
            m_equipment.armor())
        {
            result.defense = armor->defense();
        }

        for (const auto& accessory :
            m_equipment.accessories())
        {
            result.attacks +=
                accessory->attacksBonus();

            result.precision +=
                accessory->precisionBonus();
        }

        return result;
    }

    int Player::weaponDamage() const noexcept
    {
        if (const Weapon* weapon =
            m_equipment.weapon())
        {
            return weapon->damage();
        }

        return 0;
    }

    const Equipment& Player::equipment() const noexcept
    {
        return m_equipment;
    }

    Equipment& Player::equipment() noexcept
    {
        return m_equipment;
    }

    const Inventory& Player::inventory() const noexcept
    {
        return m_inventory;
    }

    Inventory& Player::inventory() noexcept
    {
        return m_inventory;
    }
}