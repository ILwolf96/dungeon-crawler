#pragma once

#include "combat/CombatStats.h"
#include "combat/CombatTarget.h"
#include "gear/Equipment.h"
#include "inventory/Inventory.h"

#include <memory>

namespace dungeon
{
    class Player final : public CombatTarget
    {
    public:
        Player();
        Player(int x, int y);

        void initialize(
            int maxHp,
            const CombatStats& baseStats,
            std::unique_ptr<Weapon> weapon,
            std::unique_ptr<Armor> armor);

        [[nodiscard]]
        int x() const noexcept override;

        [[nodiscard]]
        int y() const noexcept override;

        void setPosition(int x, int y) override;

        [[nodiscard]]
        int currentHp() const noexcept override;

        [[nodiscard]]
        int maxHp() const noexcept override;

        bool canDefend() const noexcept override;

        void takeDamage(int amount) override;

        void heal(int amount);

        bool addAccessory(std::unique_ptr<Accessory> accessory);

        [[nodiscard]]
        bool isDefeated() const noexcept override;

        [[nodiscard]]
        const char* targetType() const noexcept override;

        [[nodiscard]]
        const CombatStats& baseStats() const noexcept;

        [[nodiscard]]
        CombatStats combatStats() const noexcept override;

        [[nodiscard]]
        CombatStats effectiveStats() const noexcept;

        [[nodiscard]]
        int weaponDamage() const noexcept;

        [[nodiscard]]
        const Equipment& equipment() const noexcept;

        Equipment& equipment() noexcept;

        [[nodiscard]]
        const Inventory& inventory() const noexcept;

        Inventory& inventory() noexcept;

    private:
        int m_x{ 0 };
        int m_y{ 0 };

        int m_currentHp{ 6 };
        int m_baseMaxHp{ 6 };

        CombatStats m_baseStats{
            2, // attacks
            3, // precision
            1, // strength
            4, // toughness
            0  // defense
        };

        Equipment m_equipment;
        Inventory m_inventory;
    };
}