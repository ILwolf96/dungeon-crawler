#pragma once

#include "gear/Accessory.h"
#include "gear/Armor.h"
#include "gear/Weapon.h"

#include <memory>
#include <vector>

namespace dungeon
{
    class Equipment
    {
    public:
        Equipment() = default;

        void equipWeapon(std::unique_ptr<Weapon> weapon);
        void equipArmor(std::unique_ptr<Armor> armor);

        bool addAccessory(std::unique_ptr<Accessory> accessory);

        [[nodiscard]]
        const Weapon* weapon() const noexcept;

        [[nodiscard]]
        const Armor* armor() const noexcept;

        [[nodiscard]]
        const std::vector<std::unique_ptr<Accessory>>&
            accessories() const noexcept;

        [[nodiscard]]
        bool canEquipWeapon(const Weapon& weapon) const noexcept;

        [[nodiscard]]
        bool canEquipArmor(const Armor& armor) const noexcept;

    private:
        std::unique_ptr<Weapon> m_weapon;
        std::unique_ptr<Armor> m_armor;

        std::vector<std::unique_ptr<Accessory>> m_accessories;
    };
}