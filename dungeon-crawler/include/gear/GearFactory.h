#pragma once

#include "config/ConfigData.h"

#include <memory>
#include <string_view>

namespace dungeon
{
    class Weapon;
    class Armor;
    class Accessory;

    class GearFactory
    {
    public:
        [[nodiscard]]
        static std::unique_ptr<Weapon> createWeapon(
            const config::ConfigData& data,
            std::string_view id);

        [[nodiscard]]
        static std::unique_ptr<Armor> createArmor(
            const config::ConfigData& data,
            std::string_view id);

        [[nodiscard]]
        static std::unique_ptr<Accessory> createAccessory(
            const config::ConfigData& data,
            std::string_view id);
    };
}