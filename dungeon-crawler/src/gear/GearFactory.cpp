#include "gear/GearFactory.h"

#include "gear/Accessory.h"
#include "gear/Armor.h"
#include "gear/Weapon.h"

#include <stdexcept>
#include <string>

namespace
{
    std::string getRequiredString(
        const config::ConfigData& data,
        std::string_view section,
        std::string_view key)
    {
        const std::string sectionName(section);
        const std::string keyName(key);

        if (!data.hasValue(sectionName, keyName))
        {
            throw std::runtime_error(
                "Missing configuration value: " +
                sectionName + "." +
                keyName);
        }

        const std::string value =
            data.getValue(sectionName, keyName);

        if (value.empty())
        {
            throw std::runtime_error(
                "Configuration value cannot be empty: " +
                sectionName + "." +
                keyName);
        }

        return value;
    }

    int getRequiredInt(
        const config::ConfigData& data,
        std::string_view section,
        std::string_view key)
    {
        const std::string sectionName(section);
        const std::string keyName(key);

        if (!data.hasValue(sectionName, keyName))
        {
            throw std::runtime_error(
                "Missing configuration value: " +
                sectionName + "." +
                keyName);
        }

        try
        {
            return std::stoi(
                data.getValue(sectionName, keyName));
        }
        catch (const std::exception&)
        {
            throw std::runtime_error(
                "Invalid integer value for: " +
                sectionName + "." +
                keyName);
        }
    }

    std::string makeWeaponSection(std::string_view id)
    {
        return "gear.weapon." + std::string(id);
    }

    std::string makeArmorSection(std::string_view id)
    {
        return "gear.armor." + std::string(id);
    }

    std::string makeAccessorySection(std::string_view id)
    {
        return "gear.accessory." + std::string(id);
    }
}

namespace dungeon
{
    std::unique_ptr<Weapon> GearFactory::createWeapon(
        const config::ConfigData& data,
        std::string_view id)
    {
        const std::string section =
            makeWeaponSection(id);

        const std::string name =
            getRequiredString(data, section, "name");

        const int tier =
            getRequiredInt(data, section, "tier");

        const int strength =
            getRequiredInt(data, section, "strength");

        const int damage =
            getRequiredInt(data, section, "damage");

        return std::make_unique<Weapon>(
            name,
            tier,
            strength,
            damage);
    }

    std::unique_ptr<Armor> GearFactory::createArmor(
        const config::ConfigData& data,
        std::string_view id)
    {
        const std::string section =
            makeArmorSection(id);

        const std::string name =
            getRequiredString(data, section, "name");

        const int tier =
            getRequiredInt(data, section, "tier");

        const int defense =
            getRequiredInt(data, section, "defense");

        return std::make_unique<Armor>(
            name,
            tier,
            defense);
    }

    std::unique_ptr<Accessory> GearFactory::createAccessory(
        const config::ConfigData& data,
        std::string_view id)
    {
        const std::string section =
            makeAccessorySection(id);

        const std::string name =
            getRequiredString(data, section, "name");

        const int tier =
            getRequiredInt(data, section, "tier");

        const int attacksBonus =
            getRequiredInt(
                data,
                section,
                "attacks_bonus");

        const int precisionBonus =
            getRequiredInt(
                data,
                section,
                "precision_bonus");

        const int maxHpBonus =
            getRequiredInt(
                data,
                section,
                "max_hp_bonus");

        return std::make_unique<Accessory>(
            name,
            tier,
            attacksBonus,
            precisionBonus,
            maxHpBonus);
    }
}