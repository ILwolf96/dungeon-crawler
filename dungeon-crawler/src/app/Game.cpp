#include "app/Game.h"
#include "rendering/Renderer.h"
#include "entities/EnemyFactory.h"
#include "entities/Chest.h"
#include "config/ParserFactory.h"

#include "config/DataRepository.h"

#include "gear/GearFactory.h"
#include "loot/LootGenerator.h"
#include "loot/LootTable.h"

#include "inventory/HealthPotion.h"
#include "inventory/RagePotion.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <algorithm>


namespace
{
    constexpr std::string_view EnemySectionPrefix = "enemy.";
    constexpr float CombatInfoMessageDuration = 2.0f; // --------------- Message Duration HERE!

    bool isEnemyDefinition(std::string_view sectionName)
    {
        return sectionName.starts_with(EnemySectionPrefix);
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

    char getRequiredSymbol(
        const config::ConfigData& data,
        std::string_view section)
    {
        const std::string symbol =
            getRequiredString(data, section, "symbol");

        if (symbol.size() != 1)
        {
            throw std::runtime_error(
                "Enemy symbol must contain exactly one character: " +
                std::string(section));
        }

        return symbol.front();
    }


}

namespace dungeon
{
    Game::Game()
        : m_map(),
        m_player(0, 0)
    {
    }

    void Game::load(std::string_view filePath)
    {

        //The Old parser, keeping it for now.
        /*
        auto parser = config::makeParser(filePath);
        m_configData = parser->parse(filePath);
        */

        //The New Parser
        config::DataRepository dataRepository;
        m_configData = dataRepository.load(filePath);

        // Window configuration.
        const int windowWidth =
            getRequiredInt(m_configData, "window", "width");

        const int windowHeight =
            getRequiredInt(m_configData, "window", "height");

        const std::string title =
            getRequiredString(m_configData, "window", "title");

        std::clog << "[CONFIG] Loaded configuration: " << filePath << std::endl;
        if (windowWidth <= 0 || windowHeight <= 0)
        {
            throw std::runtime_error(
                "Window width and height must be greater than zero.");
        }

        // Map configuration.
        const std::size_t height =
            static_cast<std::size_t>(
                getRequiredInt(m_configData, "map", "height"));

        if (height == 0)
        {
            throw std::runtime_error(
                "Map height must be greater than zero.");
        }

        Map::Grid grid;
        grid.reserve(height);

        for (std::size_t row = 0; row < height; ++row)
        {
            const std::string key =
                std::string("row_") +
                (row < 10 ? "0" : "") +
                std::to_string(row);

            grid.push_back(
                getRequiredString(m_configData, "map", key));
        }

        m_map.setGrid(std::move(grid));

        // Build a lookup from map symbol to enemy definition.
        std::unordered_map<char, std::string> enemyDefinitions;

        for (const auto& [sectionName, sectionData] :
            m_configData.sections())
        {
            if (!isEnemyDefinition(sectionName))
            {
                continue;
            }

            const char symbol =
                getRequiredSymbol(m_configData, sectionName);

            if (symbol == '#' ||
                symbol == '.' ||
                symbol == 'P' ||
                symbol == 'C')
            {
                throw std::runtime_error(
                    "Enemy symbol conflicts with a reserved map symbol: " +
                    std::string(1, symbol));
            }

            if (enemyDefinitions.contains(symbol))
            {
                throw std::runtime_error(
                    "Duplicate enemy symbol: " +
                    std::string(1, symbol));
            }

            enemyDefinitions.emplace(
                symbol,
                sectionName);
        }

        // Locate the player and create entities from the map.
        bool playerFound = false;
        m_enemies.clear();
        m_chests.clear();

        for (std::size_t y = 0; y < m_map.height(); ++y)
        {
            for (std::size_t x = 0; x < m_map.width(); ++x)
            {
                const char tile =
                    m_map.tileAt(
                        static_cast<int>(x),
                        static_cast<int>(y));

                if (tile == 'P')
                {
                    if (playerFound)
                    {
                        throw std::runtime_error(
                            "Map contains more than one player.");
                    }

                    playerFound = true;

                    m_player.setPosition(
                        static_cast<int>(x),
                        static_cast<int>(y));

                    continue;
                }

                if (tile == 'C')
                {
                    m_chests.push_back(
                        std::make_unique<Chest>(
                            static_cast<int>(x),
                            static_cast<int>(y)));

                    continue;
                }

                const auto enemyDefinition =
                    enemyDefinitions.find(tile);

                if (enemyDefinition == enemyDefinitions.end())
                {
                    continue;
                }

                const std::string& sectionName =
                    enemyDefinition->second;

                const std::string type =
                    sectionName.substr(EnemySectionPrefix.size());

                const int maxHp =
                    getRequiredInt(
                        m_configData,
                        sectionName,
                        "max_hp");

                const int tier =
                    getRequiredInt(
                        m_configData,
                        sectionName,
                        "tier");

                const CombatStats stats{
                    getRequiredInt(
                        m_configData,
                        sectionName,
                        "attacks"),

                    getRequiredInt(
                        m_configData,
                        sectionName,
                        "precision"),

                    getRequiredInt(
                        m_configData,
                        sectionName,
                        "strength"),

                    getRequiredInt(
                        m_configData,
                        sectionName,
                        "toughness"),

                    getRequiredInt(
                        m_configData,
                        sectionName,
                        "defense")
                };

                auto enemy = EnemyFactory::create(
                    type,
                    maxHp,
                    tier,
                    stats,
                    static_cast<int>(x),
                    static_cast<int>(y));

                m_enemies.push_back(
                    std::move(enemy));
            }
        }

        if (!playerFound)
        {
            throw std::runtime_error(
                "Map does not contain a player.");
        }

        // Player configuration.
        const int playerMaxHp =
            getRequiredInt(m_configData, "player", "max_hp");

        const CombatStats playerBaseStats{
            getRequiredInt(m_configData, "player", "attacks"),
            getRequiredInt(m_configData, "player", "precision"),
            getRequiredInt(m_configData, "player", "strength"),
            getRequiredInt(m_configData, "player", "toughness"),
            getRequiredInt(m_configData, "player", "defense")
        };

        const std::string weaponId =
            getRequiredString(m_configData, "player", "weapon");

        const std::string armorId =
            getRequiredString(m_configData, "player", "armor");

        auto playerWeapon =
            GearFactory::createWeapon(
                m_configData,
                weaponId);

        auto playerArmor =
            GearFactory::createArmor(
                m_configData,
                armorId);

        m_player.initialize(
            playerMaxHp,
            playerBaseStats,
            std::move(playerWeapon),
            std::move(playerArmor));

        m_displayedPlayerHp =
            m_player.currentHp();

        m_displayedCombatTargetHp = 0;
        m_combatInfoMessage.clear();
        m_combatInfoMessageTime = 0.0f;
        m_pendingPotionEnemyTurn = false;

        m_healthPotionRestoreAmount =
            getRequiredInt(
                m_configData,
                "gear.consumable.health_potion",
                "restore_amount");

        m_ragePotionDamageBonus =
            getRequiredInt(
                m_configData,
                "gear.consumable.rage_potion",
                "damage_bonus");

        std::clog
            << "[CONFIG] player.precision = "
            << playerBaseStats.precision
            << std::endl;

        m_combatPresentationActor =
            CombatPresentationActor::None;

        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;
        m_title = title;
    }

    void Game::update(float deltaSeconds)
    {
        if (!m_combat)
        {
            return;
        }

        if (m_combatInfoMessageTime > 0.0f)
        {
            m_combatInfoMessageTime =
                std::max(
                    0.0f,
                    m_combatInfoMessageTime - deltaSeconds);

            if (m_combatInfoMessageTime <= 0.0f)
            {
                m_combatInfoMessage.clear();

                if (m_pendingPotionEnemyTurn)
                {
                    m_pendingPotionEnemyTurn = false;
                    beginEnemyTurnPresentation();
                }
            }
        }

        if (!m_combatPresentation.active())
        {
            return;
        }

        const bool presentationCompleted =
            m_combatPresentation.update(deltaSeconds);

        if (m_combatPresentation.phase() ==
            CombatPresentation::Phase::Result)
        {
            const AttackResult* result =
                m_combatPresentation.currentAttack();

            if (result != nullptr && result->damage > 0)
            {
                const auto rollType =
                    m_combatPresentation.rollType();

                const bool damageStep =
                    rollType == CombatPresentation::RollType::Defense
                    ||
                    (rollType == CombatPresentation::RollType::Wound &&
                        result->defenseRoll == 0);

                if (damageStep)
                {
                    const std::size_t attackIndex =
                        m_combatPresentation.currentAttackIndex();

                    if (attackIndex != m_lastPresentedDamageAttackIndex ||
                        rollType != m_lastPresentedDamageRollType)
                    {
                        if (m_combatPresentationActor ==
                            CombatPresentationActor::Player)
                        {
                            m_displayedCombatTargetHp =
                                std::max(
                                    0,
                                    m_displayedCombatTargetHp -
                                    result->damage);
                        }
                        else if (m_combatPresentationActor ==
                            CombatPresentationActor::Enemy)
                        {
                            m_displayedPlayerHp =
                                std::max(
                                    0,
                                    m_displayedPlayerHp -
                                    result->damage);
                        }

                        m_lastPresentedDamageAttackIndex =
                            attackIndex;

                        m_lastPresentedDamageRollType =
                            rollType;
                    }
                }
            }
        }

        if (presentationCompleted)
        {
            advanceCombatPresentation();
        }
    }

    void Game::beginEnemyTurnPresentation()
    {
        if (!m_combat || !m_combat->isActive())
        {
            if (m_combat != nullptr)
            {
                finishCombatIfNeeded();
            }

            return;
        }

        std::clog
            << "[COMBAT] Enemy turn begins."
            << std::endl;

        const CombatResult enemyResult =
            m_combat->enemyTurn();

        std::clog
            << "[COMBAT] "
            << enemyResult.message
            << std::endl;

        m_pendingCombatFinish =
            !m_combat->isActive();

        const auto& attackResults =
            m_combat->lastEnemyAttacks();

        if (attackResults.empty())
        {
            m_combatPresentationActor =
                CombatPresentationActor::None;

            if (m_pendingCombatFinish)
            {
                m_pendingCombatFinish = false;
                finishCombatIfNeeded();
            }

            return;
        }

        m_combatPresentationActor =
            CombatPresentationActor::Enemy;

        m_lastPresentedDamageAttackIndex =
            static_cast<std::size_t>(-1);

        m_lastPresentedDamageRollType =
            CombatPresentation::RollType::None;

        const std::string enemyName =
            std::string(
                m_combat->target().targetType());

        m_combatPresentation.start(
            enemyName,
            "Player",
            enemyName + "\nSelected Attack",
            std::vector<AttackResult>(
                attackResults.begin(),
                attackResults.end()));
    }

    void Game::advanceCombatPresentation()
    {
        if (!m_combat)
        {
            return;
        }

        if (m_pendingEnemyTurn)
        {
            m_pendingEnemyTurn = false;
            beginEnemyTurnPresentation();
            return;
        }

        if (m_pendingCombatFinish)
        {
            m_pendingCombatFinish = false;
            m_combatPresentationActor =
                CombatPresentationActor::None;

            finishCombatIfNeeded();

            return;
        }

        m_combatPresentationActor =
            CombatPresentationActor::None;
    }

    void Game::handleAction(Action action)
    {
        if (m_combat)
        {
            if (m_combatPresentation.active())
            {
                return;
            }

            if (m_inventoryOpen)
            {
                switch (action)
                {
                case Action::Attack:
                    if (useHealthPotion())
                    {
                        m_pendingPotionEnemyTurn = true;
                        m_combatInfoMessageTime =
                            CombatInfoMessageDuration;
                    }
                    break;

                case Action::Inventory:
                    if (useRagePotion())
                    {
                        m_pendingPotionEnemyTurn = true;
                        m_combatInfoMessageTime =
                            CombatInfoMessageDuration;
                    }
                    break;

                case Action::Escape:
                    closeCombatInventory();
                    break;

                default:
                    break;
                }

                return;
            }

            switch (action)
            {
            case Action::Attack:
                performCombatAttack();
                break;

            case Action::Inventory:
                openCombatInventory();
                break;

            case Action::Escape:
                performCombatEscape();
                break;

            default:
                std::clog
                    << "[COMBAT] Movement/input ignored during combat."
                    << std::endl;
                break;
            }

            return;
        }

        int targetX = m_player.x();
        int targetY = m_player.y();

        switch (action)
        {
        case Action::MoveUp:
            --targetY;
            break;

        case Action::MoveDown:
            ++targetY;
            break;

        case Action::MoveLeft:
            --targetX;
            break;

        case Action::MoveRight:
            ++targetX;
            break;

        case Action::None:
        case Action::Attack:
        case Action::Inventory:
        case Action::Escape:
            return;
        }

        if (!m_map.isWalkable(targetX, targetY))
        {
            return;
        }

        m_player.setPosition(
            targetX,
            targetY);

        CombatTarget* target =
            combatTargetAt(
                targetX,
                targetY);

        if (target != nullptr)
        {
            startCombat(*target);
        }
    }

    bool Game::inCombat() const noexcept
    {
        return m_combat != nullptr;
    }

    Combat* Game::combat() noexcept
    {
        return m_combat.get();
    }

    const Combat* Game::combat() const noexcept
    {
        return m_combat.get();
    }

    void Game::startCombat(CombatTarget& target)
    {
        if (m_combat)
        {
            return;
        }

        std::clog
            << "[COMBAT] Encounter started: "
            << target.targetType()
            << " at ("
            << target.x()
            << ", "
            << target.y()
            << ")"
            << " HP="
            << target.currentHp()
            << "/"
            << target.maxHp()
            << '\n';

        m_combat =
            std::make_unique<Combat>(
                m_player,
                target,
                m_combatDice);

        m_displayedPlayerHp =
            m_player.currentHp();

        m_displayedCombatTargetHp =
            target.currentHp();

        // -------------------------------------------------------------------------
        // Loot preview
        //
        // Enemies use their configured loot tier.
        // Chests currently use Tier 1.
        // This only reads the loot table; it does not roll or award anything.
        // -------------------------------------------------------------------------

        int lootTier = 1;

        for (const auto& enemy : m_enemies)
        {
            if (enemy.get() != &target)
            {
                continue;
            }

            lootTier =
                enemy->tier();

            break;
        }

        m_combatLootPreview =
            buildCombatLootPreview(
                target,
                lootTier);
    }

    std::vector<CombatLootPreviewSlot>
        Game::buildCombatLootPreview(
            const CombatTarget& target,
            int lootTier) const
    {
        constexpr std::size_t PreviewSlotCount = 5;

        std::vector<CombatLootPreviewSlot> preview;
        preview.reserve(
            PreviewSlotCount);

        const auto makeNone = []()
            {
                return CombatLootPreviewSlot{
                    CombatLootPreviewType::None,
                    {},
                    0
                };
            };

        const auto appendNone = [&]()
            {
                if (preview.size() < PreviewSlotCount)
                {
                    preview.push_back(
                        makeNone());
                }
            };

        const auto appendHealthPotion = [&]()
            {
                if (preview.size() < PreviewSlotCount)
                {
                    preview.push_back(
                        CombatLootPreviewSlot{
                            CombatLootPreviewType::HealthPotion,
                            "health_potion",
                            0
                        });
                }
            };

        const auto appendRagePotion = [&]()
            {
                if (preview.size() < PreviewSlotCount)
                {
                    preview.push_back(
                        CombatLootPreviewSlot{
                            CombatLootPreviewType::RagePotion,
                            "rage_potion",
                            0
                        });
                }
            };

        // Chests have no gear candidates in the current loot table.
        // Their five-slot preview is explicitly:
        // Health Potion, Rage Potion, None, None, None.
        if (dynamic_cast<const Chest*>(&target) != nullptr)
        {
            appendHealthPotion();
            appendRagePotion();

            while (preview.size() < PreviewSlotCount)
            {
                appendNone();
            }

            return preview;
        }

        const auto& equippedWeapon =
            m_player.equipment().weapon();

        const auto& equippedArmor =
            m_player.equipment().armor();

        const auto& equippedAccessories =
            m_player.equipment().accessories();

        const auto accessoryAlreadyOwned =
            [&](std::string_view rewardId)
            {
                for (const auto& accessory : equippedAccessories)
                {
                    if (!accessory)
                    {
                        continue;
                    }

                    const std::string_view name =
                        accessory->name();

                    if ((rewardId == "magic_skull" &&
                        name == "Magic Skull") ||
                        (rewardId == "orc_fang" &&
                            name == "Orc Fang") ||
                        (rewardId == "troll_heart" &&
                            name == "Troll's Heart"))
                    {
                        return true;
                    }
                }

                return false;
            };

        const auto appendGearReward =
            [&](const LootReward& reward)
            {
                bool available = false;

                switch (reward.type)
                {
                case LootType::Weapon:
                    available =
                        equippedWeapon == nullptr ||
                        equippedWeapon->tier() < reward.tier;
                    break;

                case LootType::Armor:
                    available =
                        equippedArmor == nullptr ||
                        equippedArmor->tier() < reward.tier;
                    break;

                case LootType::Accessory:
                    available =
                        equippedAccessories.size() < 3 &&
                        !accessoryAlreadyOwned(
                            reward.id);
                    break;
                }

                if (!available)
                {
                    appendNone();
                    return;
                }

                CombatLootPreviewSlot slot;
                slot.id = reward.id;
                slot.tier = reward.tier;

                switch (reward.type)
                {
                case LootType::Weapon:
                    slot.type =
                        CombatLootPreviewType::Weapon;
                    break;

                case LootType::Armor:
                    slot.type =
                        CombatLootPreviewType::Armor;
                    break;

                case LootType::Accessory:
                    slot.type =
                        CombatLootPreviewType::Accessory;
                    break;
                }

                if (preview.size() < PreviewSlotCount)
                {
                    preview.push_back(
                        std::move(slot));
                }
            };

        const std::vector<LootReward> rewards =
            LootTable::rewardsForTier(
                lootTier);

        for (const LootReward& reward : rewards)
        {
            appendGearReward(
                reward);

            if (preview.size() >= PreviewSlotCount)
            {
                break;
            }
        }

        // Potions are the two remaining possible reward categories.
        appendHealthPotion();
        appendRagePotion();

        while (preview.size() < PreviewSlotCount)
        {
            appendNone();
        }

        return preview;
    }

    CombatTarget* Game::combatTargetAt(
        int x,
        int y) noexcept
    {
        for (const auto& enemy : m_enemies)
        {
            if (enemy->x() == x &&
                enemy->y() == y &&
                !enemy->isDefeated())
            {
                return enemy.get();
            }
        }

        for (const auto& chest : m_chests)
        {
            if (chest->x() == x &&
                chest->y() == y &&
                !chest->isDefeated())
            {
                return chest.get();
            }
        }

        return nullptr;
    }

    void Game::performCombatAttack()
    {
        if (!m_combat ||
            m_combatPresentation.active())
        {
            return;
        }

        std::clog
            << "[COMBAT] Player selected Attack.\n";

        const CombatResult playerResult =
            m_combat->playerAttack();

        std::clog
            << "[COMBAT] "
            << playerResult.message
            << '\n';

        const auto& attackResults =
            m_combat->lastPlayerAttacks();

        m_pendingEnemyTurn =
            m_combat->isActive();

        m_pendingCombatFinish =
            !m_combat->isActive();

        m_combatPresentationActor =
            CombatPresentationActor::Player;

        m_lastPresentedDamageAttackIndex =
            static_cast<std::size_t>(-1);

        m_lastPresentedDamageRollType =
            CombatPresentation::RollType::None;

        m_combatPresentation.start(
            "Player",
            std::string(
                m_combat->target().targetType()),
            "Player\nSelected Attack",
            std::vector<AttackResult>(
                attackResults.begin(),
                attackResults.end()));
    }

    void Game::performCombatEscape()
    {
        if (!m_combat)
        {
            return;
        }

        std::clog
            << "[COMBAT] Player selected Escape.\n";

        const CombatResult result =
            m_combat->escape();

        std::clog
            << "[COMBAT] "
            << result.message
            << '\n';

        finishCombatIfNeeded();
    }

    void Game::openCombatInventory()
    {
        if (!m_combat)
        {
            return;
        }

        m_inventoryOpen = true;

        m_combatInfoMessage =
            "Opening Inventory";

        m_combatInfoMessageTime =
            CombatInfoMessageDuration;

        std::clog
            << "[INVENTORY] ====================\n"
            << "[INVENTORY] Combat Inventory\n"
            << "[INVENTORY] Health Potion: "
            << m_player.inventory().amount(HealthPotion{})
            << "/3\n"
            << "[INVENTORY] Rage Potion: "
            << m_player.inventory().amount(RagePotion{})
            << "/3\n"
            << "[INVENTORY] Player HP: "
            << m_player.currentHp()
            << "/"
            << m_player.maxHp()
            << '\n'
            << "[INVENTORY] Rage Bonus: +"
            << m_combat->playerDamageBonus()
            << " damage\n"
            << "[INVENTORY] 1 = Health Potion\n"
            << "[INVENTORY] 2 = Rage Potion\n"
            << "[INVENTORY] 3 = Close Inventory\n"
            << "[INVENTORY] ====================\n";
    }

    void Game::closeCombatInventory()
    {
        if (!m_inventoryOpen)
        {
            return;
        }

        m_inventoryOpen = false;

        m_combatInfoMessage =
            "Closing Inventory";

        m_combatInfoMessageTime =
            CombatInfoMessageDuration;

        std::clog
            << "[INVENTORY] Combat inventory closed.\n";
    }

    bool Game::useHealthPotion()
    {
        if (!m_combat)
        {
            return false;
        }


        const int currentHp = m_player.currentHp();
        const int maximumHp = m_player.maxHp();

        if (!m_player.inventory().hasConsumable(HealthPotion{}))
        {
            std::clog << "[INVENTORY] Cannot use Health Potion: none available." << std::endl;

            m_combatInfoMessage =
                "No Health Potion available";

            m_combatInfoMessageTime =
                CombatInfoMessageDuration;

            return false;
        }

        if (currentHp >= maximumHp)
        {
            std::clog
                << "[INVENTORY] Cannot use Health Potion: Player HP is already full ("
                << currentHp << "/" << maximumHp << ")." << std::endl;
            return false;
        }

        m_player.inventory().removeConsumable(HealthPotion{});

        const int hpBefore = m_player.currentHp();
        m_player.heal(m_healthPotionRestoreAmount);
        const int hpAfter = m_player.currentHp();
        const int restored = hpAfter - hpBefore;

        m_displayedPlayerHp = hpAfter;

        m_combatInfoMessage =
            "Player used Health Potion, HP Restored by " +
            std::to_string(restored);

        return true;
    }

    bool Game::useRagePotion()
    {
        if (!m_combat)
        {
            return false;
        }


        if (!m_player.inventory().hasConsumable(RagePotion{}))
        {
            std::clog << "[INVENTORY] Cannot use Rage Potion: none available." << std::endl;

            m_combatInfoMessage =
                "No Rage Potion available";

            m_combatInfoMessageTime =
                CombatInfoMessageDuration;

            return false;
        }

        m_player.inventory().removeConsumable(RagePotion{});
        const int damageBefore = m_combat->playerDamageBonus();
        m_combat->addPlayerDamageBonus(m_ragePotionDamageBonus);

        const int increasedDamage =
            m_combat->playerDamageBonus() - damageBefore;

        m_combatInfoMessage =
            "Player used Rage Potion, DMG Increased by " +
            std::to_string(increasedDamage);

        return true;
    }

    void Game::finishCombatIfNeeded()
    {
        if (!m_combat ||
            m_combat->isActive())
        {
            return;
        }

        m_inventoryOpen = false;
        m_combatLootPreview.clear();

        CombatTarget* target =
            &m_combat->target();

        const bool targetDefeated =
            target->isDefeated();

        m_combat.reset();

        if (!targetDefeated)
        {
            std::clog
                << "[COMBAT] Encounter ended without target defeat.\n";

            return;
        }

        std::string lootMessage;

        for (auto it = m_enemies.begin();
            it != m_enemies.end();
            ++it)
        {
            if (it->get() != target)
            {
                continue;
            }

            const int tier =
                (*it)->tier();

            std::clog
                << "[LOOT] Defeated "
                << (*it)->targetType()
                << " Tier "
                << tier
                << ". Generating reward...\n";

            static_cast<void>(
                LootGenerator::award(
                    m_player,
                    tier,
                    m_combatDice,
                    m_configData,
                    lootMessage));

            std::clog
                << "[LOOT] "
                << lootMessage
                << '\n';

            static_cast<void>(
                m_enemies.erase(it));

            std::clog
                << "[COMBAT] Enemy removed from map.\n";

            return;
        }

        for (auto it = m_chests.begin();
            it != m_chests.end();
            ++it)
        {
            if (it->get() != target)
            {
                continue;
            }

            const int tier = 1;

            std::clog
                << "[LOOT] Chest opened. Generating reward...\n";

            static_cast<void>(
                LootGenerator::award(
                    m_player,
                    tier,
                    m_combatDice,
                    m_configData,
                    lootMessage));

            std::clog
                << "[LOOT] "
                << lootMessage
                << '\n';

            static_cast<void>(
                m_chests.erase(it));

            std::clog
                << "[COMBAT] Chest removed from map.\n";

            return;
        }
    }

    // ----------------------------------------------- Thy Getters Here (tired of forgetting)
    bool Game::combatPresentationActive() const noexcept
    {
        return m_combatPresentation.active();
    }

    const CombatPresentation&
        Game::combatPresentation() const noexcept
    {
        return m_combatPresentation;
    }

    int Game::windowWidth() const noexcept
    {
        return m_windowWidth;
    }

    int Game::windowHeight() const noexcept
    {
        return m_windowHeight;
    }

    const std::string& Game::title() const noexcept
    {
        return m_title;
    }

    const Map& Game::map() const noexcept
    {
        return m_map;
    }

    Player& Game::player() noexcept
    {
        return m_player;
    }

    const Player& Game::player() const noexcept
    {
        return m_player;
    }

    const std::vector<std::unique_ptr<Enemy>>&
        Game::enemies() const noexcept
    {
        return m_enemies;
    }

    const std::vector<std::unique_ptr<Chest>>&
        Game::chests() const noexcept
    {
        return m_chests;
    }

    int Game::displayedPlayerHp() const noexcept
    {
        if (!m_combat)
        {
            return m_player.currentHp();
        }

        return m_displayedPlayerHp;
    }

    int Game::displayedCombatTargetHp() const noexcept
    {
        if (!m_combat)
        {
            return 0;
        }

        return m_displayedCombatTargetHp;
    }

    bool Game::inventoryOpen() const noexcept
    {
        return m_inventoryOpen;
    }

    const std::string&
        Game::combatInfoMessage() const noexcept
    {
        return m_combatInfoMessage;
    }

    const std::vector<CombatLootPreviewSlot>&
        Game::combatLootPreview() const noexcept
    {
        return m_combatLootPreview;
    }
}