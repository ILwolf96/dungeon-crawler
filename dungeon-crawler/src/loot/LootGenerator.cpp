#include "loot/LootGenerator.h"

#include "combat/IDice.h"
#include "entities/Player.h"
#include "gear/GearFactory.h"
#include "inventory/HealthPotion.h"
#include "inventory/RagePotion.h"
#include "loot/LootTable.h"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    std::string gearSection(
        dungeon::LootType type,
        std::string_view id)
    {
        switch (type)
        {
        case dungeon::LootType::Weapon:
            return "gear.weapon." + std::string(id);

        case dungeon::LootType::Armor:
            return "gear.armor." + std::string(id);

        case dungeon::LootType::Accessory:
            return "gear.accessory." + std::string(id);
        }

        throw std::logic_error(
            "Unknown loot type.");
    }

    std::string requiredString(
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
                sectionName + "." + keyName);
        }

        return data.getValue(sectionName, keyName);
    }

    bool accessoryAlreadyCollected(
        const dungeon::Player& player,
        const dungeon::LootReward& reward,
        const config::ConfigData& data)
    {
        const std::string section =
            gearSection(reward.type, reward.id);

        const std::string rewardName =
            requiredString(data, section, "name");

        for (const auto& accessory :
            player.equipment().accessories())
        {
            if (accessory->name() == rewardName)
            {
                return true;
            }
        }

        return false;
    }

    bool rewardIsAvailable(
        const dungeon::Player& player,
        const dungeon::LootReward& reward,
        const config::ConfigData& data)
    {
        switch (reward.type)
        {
        case dungeon::LootType::Weapon:
        {
            const dungeon::Weapon* weapon =
                player.equipment().weapon();

            return weapon == nullptr ||
                weapon->tier() < reward.tier;
        }

        case dungeon::LootType::Armor:
        {
            const dungeon::Armor* armor =
                player.equipment().armor();

            return armor == nullptr ||
                armor->tier() < reward.tier;
        }

        case dungeon::LootType::Accessory:
        {
            if (player.equipment().accessories().size() >= 3)
            {
                return false;
            }

            return !accessoryAlreadyCollected(
                player,
                reward,
                data);
        }
        }

        return false;
    }

    std::size_t selectIndex(
        std::size_t count,
        dungeon::IDice& dice)
    {
        if (count == 0)
        {
            throw std::invalid_argument(
                "Cannot select loot from an empty collection.");
        }

        if (count == 1)
        {
            return 0;
        }

        if (count > 6)
        {
            throw std::invalid_argument(
                "Loot collection cannot contain more than six rewards.");
        }

        const int countAsInt =
            static_cast<int>(count);

        const int largestAcceptableRoll =
            6 - (6 % countAsInt);

        int roll = 0;

        do
        {
            roll = dice.rollD6();
        } while (roll > largestAcceptableRoll);

        return static_cast<std::size_t>(
            (roll - 1) % countAsInt);
    }

    bool awardFallbackConsumable(
        dungeon::Player& player,
        dungeon::IDice& dice,
        std::string& message)
    {
        dungeon::Inventory& inventory =
            player.inventory();

        const dungeon::HealthPotion healthPotion;
        const dungeon::RagePotion ragePotion;

        const bool canHealth =
            inventory.canAddConsumable(
                healthPotion);

        const bool canRage =
            inventory.canAddConsumable(
                ragePotion);

        if (!canHealth && !canRage)
        {
            message =
                "No consumable inventory space remains.";

            return false;
        }

        if (canHealth && canRage)
        {
            const std::size_t selected =
                selectIndex(2, dice);

            if (selected == 0)
            {
                inventory.addConsumable(
                    std::make_unique<
                    dungeon::HealthPotion>());

                message =
                    "Loot: Health Potion.";

                return true;
            }

            inventory.addConsumable(
                std::make_unique<
                dungeon::RagePotion>());

            message =
                "Loot: Rage Potion.";

            return true;
        }

        if (canHealth)
        {
            inventory.addConsumable(
                std::make_unique<
                dungeon::HealthPotion>());

            message =
                "Loot: Health Potion.";

            return true;
        }

        inventory.addConsumable(
            std::make_unique<
            dungeon::RagePotion>());

        message =
            "Loot: Rage Potion.";

        return true;
    }

    bool awardGear(
        dungeon::Player& player,
        const dungeon::LootReward& reward,
        const config::ConfigData& data,
        std::string& message)
    {
        switch (reward.type)
        {
        case dungeon::LootType::Weapon:
        {
            auto weapon =
                dungeon::GearFactory::createWeapon(
                    data,
                    reward.id);

            if (!player.equipment().canEquipWeapon(
                *weapon))
            {
                return false;
            }

            const std::string name =
                weapon->name();

            player.equipment().equipWeapon(
                std::move(weapon));

            message =
                "Loot: " + name + ".";

            return true;
        }

        case dungeon::LootType::Armor:
        {
            auto armor =
                dungeon::GearFactory::createArmor(
                    data,
                    reward.id);

            if (!player.equipment().canEquipArmor(
                *armor))
            {
                return false;
            }

            const std::string name =
                armor->name();

            player.equipment().equipArmor(
                std::move(armor));

            message =
                "Loot: " + name + ".";

            return true;
        }

        case dungeon::LootType::Accessory:
        {
            auto accessory =
                dungeon::GearFactory::createAccessory(
                    data,
                    reward.id);

            const std::string name =
                accessory->name();

            if (!player.equipment().addAccessory(
                std::move(accessory)))
            {
                return false;
            }

            message =
                "Loot: " + name + ".";

            return true;
        }
        }

        return false;
    }
}

namespace dungeon
{
    bool LootGenerator::award(
        Player& player,
        int tier,
        IDice& dice,
        const config::ConfigData& data,
        std::string& message)
    {
        const std::vector<LootReward> allRewards =
            LootTable::rewardsForTier(tier);

        std::vector<LootReward> availableRewards;
        availableRewards.reserve(allRewards.size());

        for (const LootReward& reward : allRewards)
        {
            if (rewardIsAvailable(
                player,
                reward,
                data))
            {
                availableRewards.push_back(reward);
            }
        }

        if (availableRewards.empty())
        {
            return awardFallbackConsumable(
                player,
                dice,
                message);
        }

        const std::size_t selectedIndex =
            selectIndex(
                availableRewards.size(),
                dice);

        const LootReward& reward =
            availableRewards[selectedIndex];

        if (awardGear(
            player,
            reward,
            data,
            message))
        {
            return true;
        }

        return awardFallbackConsumable(
            player,
            dice,
            message);
    }
}