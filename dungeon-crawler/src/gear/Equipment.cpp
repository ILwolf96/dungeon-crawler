#include "gear/Equipment.h"

namespace dungeon
{
    void Equipment::equipWeapon(
        std::unique_ptr<Weapon> weapon)
    {
        if (!weapon)
        {
            return;
        }

        if (!canEquipWeapon(*weapon))
        {
            return;
        }

        m_weapon = std::move(weapon);
    }

    void Equipment::equipArmor(
        std::unique_ptr<Armor> armor)
    {
        if (!armor)
        {
            return;
        }

        if (!canEquipArmor(*armor))
        {
            return;
        }

        m_armor = std::move(armor);
    }

    bool Equipment::addAccessory(
        std::unique_ptr<Accessory> accessory)
    {
        if (!accessory)
        {
            return false;
        }

        for (const auto& existing : m_accessories)
        {
            if (existing->name() == accessory->name())
            {
                return false;
            }
        }

        if (m_accessories.size() >= 3)
        {
            return false;
        }

        m_accessories.push_back(
            std::move(accessory));

        return true;
    }

    const Weapon* Equipment::weapon() const noexcept
    {
        return m_weapon.get();
    }

    const Armor* Equipment::armor() const noexcept
    {
        return m_armor.get();
    }

    const std::vector<std::unique_ptr<Accessory>>&
        Equipment::accessories() const noexcept
    {
        return m_accessories;
    }

    bool Equipment::canEquipWeapon(
        const Weapon& weapon) const noexcept
    {
        return !m_weapon ||
            weapon.tier() > m_weapon->tier();
    }

    bool Equipment::canEquipArmor(
        const Armor& armor) const noexcept
    {
        return !m_armor ||
            armor.tier() > m_armor->tier();
    }


    //---------------------------------------------- DEBUG FOR ZOO HHERE!

    void Equipment::debugEquipWeapon(std::unique_ptr<Weapon> weapon) noexcept
    {
        m_weapon = std::move(weapon);
    }

    void Equipment::debugEquipArmor(std::unique_ptr<Armor> armor) noexcept
    {
        m_armor = std::move(armor);
    }

    bool Equipment::debugRemoveAccessory(std::string_view accessoryName) noexcept
    {
        for (auto it = m_accessories.begin(); it != m_accessories.end(); ++it)
        {
            if (!*it)
            {
                continue;
            }

            if ((*it)->name() == accessoryName)
            {
                m_accessories.erase(it);
                return true;
            }
        }

        return false;
    }


}