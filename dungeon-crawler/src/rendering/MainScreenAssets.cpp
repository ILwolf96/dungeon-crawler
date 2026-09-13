#include "rendering/MainScreenAssets.h"

#include "raylib.h"

#include <array>
#include <string>
#include <string_view>

namespace
{
    constexpr const char* AssetRoot = "assets/ui/";

    constexpr const char* TitlesPath =
        "assets/ui/titles/";

    constexpr const char* StatsIconsPath =
        "assets/ui/stats/icons/";

    constexpr const char* StatsNumbersPath =
        "assets/ui/stats/numbers/";

    constexpr const char* PotionsPath =
        "assets/ui/gear/potions/";

    constexpr const char* WeaponsPath =
        "assets/ui/gear/weapons/";

    constexpr const char* ArmorPath =
        "assets/ui/gear/armor/";

    constexpr const char* AccessoriesPath =
        "assets/ui/gear/accessories/";

    constexpr const char* MapPath =
        "assets/ui/map/";

    constexpr const char* ActionBarPath =
        "assets/ui/action_bar/";

    constexpr const char* WindowsPath =
        "assets/ui/windows/";

    constexpr const char* InstructionsPath =
        "assets/ui/instructions/";

    constexpr const char* DicePath =
        "assets/ui/dice/";

    constexpr const char* PovTraversalPath =
        "assets/ui/pov/traversal/";

    constexpr const char* PovCombatPath =
        "assets/ui/pov/combat/";

    constexpr const char* EnemyPovNames[] =
    {
        "goblin",
        "skeleton",
        "orc",
        "troll",
        "dragon"
    };

    /*
    constexpr const char* MapPath =
        "assets/ui/map/";

    constexpr const char* ActionBarPath =
        "assets/ui/action_bar/";

    constexpr const char* PovTraversalPath =
        "assets/ui/pov/traversal/";

    constexpr const char* PovCombatPath =
        "assets/ui/pov/combat/";
    */
}

namespace dungeon
{
    MainScreenAssets::~MainScreenAssets()
    {
        unload();
    }

    bool MainScreenAssets::isValidTexture(
        const Texture2D& texture) noexcept
    {
        return texture.id != 0;
    }

    int MainScreenAssets::enemyPovIndex(
        std::string_view type) noexcept
    {
        if (type == "Goblin")
        {
            return 0;
        }

        if (type == "Skeleton")
        {
            return 1;
        }

        if (type == "Orc")
        {
            return 2;
        }

        if (type == "Troll")
        {
            return 3;
        }

        if (type == "Dragon")
        {
            return 4;
        }

        return -1;
    }

    Texture2D MainScreenAssets::emptyTexture() noexcept
    {
        return Texture2D{};
    }

    Texture2D MainScreenAssets::loadTextureIfPresent(
        const std::string& path,
        bool& loadedSuccessfully)
    {
        if (!FileExists(path.c_str()))
        {
            loadedSuccessfully = false;
            return emptyTexture();
        }

        Texture2D texture = LoadTexture(path.c_str());

        if (!isValidTexture(texture))
        {
            loadedSuccessfully = false;
            return emptyTexture();
        }

        loadedSuccessfully = true;
        return texture;
    }

    bool MainScreenAssets::load()
    {
        unload();

        bool allLoaded = true;
        bool loaded = false;

        // ---------------------------------------------------------------------
        // Titles
        // ---------------------------------------------------------------------

        m_titleArt = loadTextureIfPresent(
            std::string(TitlesPath) + "title_art.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_statsTitle = loadTextureIfPresent(
            std::string(TitlesPath) + "stats_title.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_gearTitle = loadTextureIfPresent(
            std::string(TitlesPath) + "gear_title.png",
            loaded);
        allLoaded = allLoaded && loaded;

        // ---------------------------------------------------------------------
        // Window / Panel Art
        // ---------------------------------------------------------------------

        m_playerStatsWindowBackground = loadTextureIfPresent(
            std::string(WindowsPath) + "player_stats_window_bg.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_gearWindowBackground = loadTextureIfPresent(
            std::string(WindowsPath) + "gear_window_bg.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_diceRollTitle = loadTextureIfPresent(
            std::string(WindowsPath) + "dice_roll_title.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_diceRollWindowBackground = loadTextureIfPresent(
            std::string(WindowsPath) + "dice_roll_window_bg.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_combatInfoWindowBackground = loadTextureIfPresent(
            std::string(WindowsPath) + "combat_info_window_bg.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_lootTableWindowBackground = loadTextureIfPresent(
            std::string(WindowsPath) + "loot_table_window_bg.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_lootTitle = loadTextureIfPresent(
            std::string(WindowsPath) + "loot_title.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_enemyStatsWindowBackground = loadTextureIfPresent(
            std::string(WindowsPath) + "enemy_stats_window_bg.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_enemyStatsTitle = loadTextureIfPresent(
            std::string(WindowsPath) + "enemy_stats_title.png",
            loaded);
        allLoaded = allLoaded && loaded;

        // ---------------------------------------------------------------------
        // Stat Icons
        // ---------------------------------------------------------------------

        m_atkIcon = loadTextureIfPresent(
            std::string(StatsIconsPath) + "atk_icon.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_strIcon = loadTextureIfPresent(
            std::string(StatsIconsPath) + "str_icon.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_precIcon = loadTextureIfPresent(
            std::string(StatsIconsPath) + "prec_icon.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_dmgIcon = loadTextureIfPresent(
            std::string(StatsIconsPath) + "dmg_icon.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_defIcon = loadTextureIfPresent(
            std::string(StatsIconsPath) + "def_icon.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_toughIcon = loadTextureIfPresent(
            std::string(StatsIconsPath) + "tough_icon.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_hpIcon = loadTextureIfPresent(
            std::string(StatsIconsPath) + "hp_icon.png",
            loaded);
        allLoaded = allLoaded && loaded;

        // ---------------------------------------------------------------------
        // Numbers 0-16
        // ---------------------------------------------------------------------

        for (int value = 0; value < NumberAssetCount; ++value)
        {
            m_numbers[static_cast<std::size_t>(value)] =
                loadTextureIfPresent(
                    std::string(StatsNumbersPath) +
                    "num" +
                    std::to_string(value) +
                    ".png",
                    loaded);

            allLoaded = allLoaded && loaded;
        }

        // ---------------------------------------------------------------------
        // Potion Icons
        // ---------------------------------------------------------------------

        m_healthPotionIcon = loadTextureIfPresent(
            std::string(PotionsPath) + "health_potion_icon.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_ragePotionIcon = loadTextureIfPresent(
            std::string(PotionsPath) + "rage_potion_icon.png",
            loaded);
        allLoaded = allLoaded && loaded;

        // ---------------------------------------------------------------------
        // Potion Amounts
        // ---------------------------------------------------------------------

        for (int amount = 0; amount < PotionStateCount; ++amount)
        {
            m_healthPotionAmounts[
                static_cast<std::size_t>(amount)] =
                loadTextureIfPresent(
                    std::string(PotionsPath) +
                    "health_" +
                    std::to_string(amount) +
                    "_3.png",
                    loaded);

                allLoaded = allLoaded && loaded;

                m_ragePotionAmounts[
                    static_cast<std::size_t>(amount)] =
                    loadTextureIfPresent(
                        std::string(PotionsPath) +
                        "rage_" +
                        std::to_string(amount) +
                        "_3.png",
                        loaded);

                    allLoaded = allLoaded && loaded;
        }

        // ---------------------------------------------------------------------
        // Weapons
        // ---------------------------------------------------------------------

        for (int tier = 1; tier <= GearTierCount; ++tier)
        {
            m_weaponTiers[
                static_cast<std::size_t>(tier - 1)] =
                loadTextureIfPresent(
                    std::string(WeaponsPath) +
                    "weap_tier_" +
                    std::to_string(tier) +
                    ".png",
                    loaded);

                allLoaded = allLoaded && loaded;
        }

        // ---------------------------------------------------------------------
        // Armor
        // ---------------------------------------------------------------------

        for (int tier = 1; tier <= GearTierCount; ++tier)
        {
            m_armorTiers[
                static_cast<std::size_t>(tier - 1)] =
                loadTextureIfPresent(
                    std::string(ArmorPath) +
                    "armor_tier_" +
                    std::to_string(tier) +
                    ".png",
                    loaded);

                allLoaded = allLoaded && loaded;
        }

        // ---------------------------------------------------------------------
        // Accessories
        // ---------------------------------------------------------------------

        m_noAccessory = loadTextureIfPresent(
            std::string(AccessoriesPath) + "no_accessory.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_magicSkull = loadTextureIfPresent(
            std::string(AccessoriesPath) + "magic_skull.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_orcFang = loadTextureIfPresent(
            std::string(AccessoriesPath) + "orc_fang.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_trollHeart = loadTextureIfPresent(
            std::string(AccessoriesPath) + "troll_heart.png",
            loaded);
        allLoaded = allLoaded && loaded;

        // ---------------------------------------------------------------------
        // Map
        // ---------------------------------------------------------------------

        m_wallTile = loadTextureIfPresent(
            std::string(MapPath) + "wall_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_floorTile = loadTextureIfPresent(
            std::string(MapPath) + "floor_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_playerTile = loadTextureIfPresent(
            std::string(MapPath) + "player_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_chestTile = loadTextureIfPresent(
            std::string(MapPath) + "chest_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_chestOpenedTile = loadTextureIfPresent(
            std::string(MapPath) + "chest_opened_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_goblinTile = loadTextureIfPresent(
            std::string(MapPath) + "goblin_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_goblinDefeatedTile = loadTextureIfPresent(
            std::string(MapPath) + "goblin_defeated_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_skeletonTile = loadTextureIfPresent(
            std::string(MapPath) + "skeleton_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_skeletonDefeatedTile = loadTextureIfPresent(
            std::string(MapPath) + "skeleton_defeated_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_orcTile = loadTextureIfPresent(
            std::string(MapPath) + "orc_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_orcDefeatedTile = loadTextureIfPresent(
            std::string(MapPath) + "orc_defeated_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_trollTile = loadTextureIfPresent(
            std::string(MapPath) + "troll_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_trollDefeatedTile = loadTextureIfPresent(
            std::string(MapPath) + "troll_defeated_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_dragonTile = loadTextureIfPresent(
            std::string(MapPath) + "dragon_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_dragonDefeatedTile = loadTextureIfPresent(
            std::string(MapPath) + "dragon_defeated_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_mapFrame = loadTextureIfPresent(
            std::string(MapPath) + "map_frame.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_portalTile = loadTextureIfPresent(
            std::string(MapPath) + "portal_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        // ---------------------------------------------------------------------
        // POV - Traversal
        // ---------------------------------------------------------------------

        for (int index = 0; index < EnemyPovTypeCount; ++index)
        {
            const std::string enemyName =
                EnemyPovNames[index];

            m_seesEnemies[static_cast<std::size_t>(index)] =
                loadTextureIfPresent(
                    std::string(PovTraversalPath) +
                    "sees_" +
                    enemyName +
                    ".png",
                    loaded);
            allLoaded = allLoaded && loaded;

            m_seesDefeatedEnemies[static_cast<std::size_t>(index)] =
                loadTextureIfPresent(
                    std::string(PovTraversalPath) +
                    "sees_defeated_" +
                    enemyName +
                    ".png",
                    loaded);
            allLoaded = allLoaded && loaded;
        }

        m_seesChest = loadTextureIfPresent(
            std::string(PovTraversalPath) +
            "sees_chest.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_seesDefeatedChest = loadTextureIfPresent(
            std::string(PovTraversalPath) +
            "sees_defeated_chest.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_seesWall = loadTextureIfPresent(
            std::string(PovTraversalPath) +
            "sees_wall.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_seesNothing = loadTextureIfPresent(
            std::string(PovTraversalPath) +
            "sees_nothing.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_portal = loadTextureIfPresent(
            std::string(PovTraversalPath) + "portal.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_seesPortal = loadTextureIfPresent(
            std::string(PovTraversalPath) + "sees_portal.png",
            loaded);
        allLoaded = allLoaded && loaded;

        // ---------------------------------------------------------------------
        // POV - Combat
        // ---------------------------------------------------------------------

        m_chestIdle =
            loadTextureIfPresent(
                std::string(PovCombatPath) +
                "chest_idle.png",
                loaded);
        allLoaded = allLoaded && loaded;

        m_chestDeath =
            loadTextureIfPresent(
                std::string(PovCombatPath) +
                "chest_death.png",
                loaded);
        allLoaded = allLoaded && loaded;

        for (int index = 0; index < EnemyPovTypeCount; ++index)
        {
            const std::string enemyName =
                EnemyPovNames[index];

            m_enemyIdle[static_cast<std::size_t>(index)] =
                loadTextureIfPresent(
                    std::string(PovCombatPath) +
                    enemyName +
                    "_idle.png",
                    loaded);
            allLoaded = allLoaded && loaded;

            m_enemyHurt[static_cast<std::size_t>(index)] =
                loadTextureIfPresent(
                    std::string(PovCombatPath) +
                    enemyName +
                    "_hurt.png",
                    loaded);
            allLoaded = allLoaded && loaded;

            m_enemyAttack[static_cast<std::size_t>(index)] =
                loadTextureIfPresent(
                    std::string(PovCombatPath) +
                    enemyName +
                    "_attack.png",
                    loaded);
            allLoaded = allLoaded && loaded;

            m_enemyDeath[static_cast<std::size_t>(index)] =
                loadTextureIfPresent(
                    std::string(PovCombatPath) +
                    enemyName +
                    "_death.png",
                    loaded);
            allLoaded = allLoaded && loaded;

            m_enemyVictory[
                static_cast<std::size_t>(index)] =
                loadTextureIfPresent(
                    std::string(PovCombatPath) +
                    enemyName +
                    "_victory.png",
                    loaded);

                allLoaded = allLoaded && loaded;
        }

        // ---------------------------------------------------------------------
        // Instructions
        // ---------------------------------------------------------------------

        m_gameInstructions = loadTextureIfPresent(
            std::string(InstructionsPath) + "game_instructions.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_combatInstructions = loadTextureIfPresent(
            std::string(InstructionsPath) + "combat_instructions.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_inventoryInstructions = loadTextureIfPresent(
            std::string(InstructionsPath) + "inventory_instructions.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_gameInstructionsBackground = loadTextureIfPresent(
            std::string(InstructionsPath) + "game_instructions_bg.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_combatInstructionsBackground = loadTextureIfPresent(
            std::string(InstructionsPath) + "combat_instructions_bg.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_inventoryInstructionsBackground = loadTextureIfPresent(
            std::string(InstructionsPath) + "inventory_instructions_bg.png",
            loaded);
        allLoaded = allLoaded && loaded;

        // ---------------------------------------------------------------------
        // Dice Presentation
        // ---------------------------------------------------------------------

        m_diceRolling = loadTextureIfPresent(
            std::string(DicePath) + "dice_rolling.png",
            loaded);
        allLoaded = allLoaded && loaded;

        for (int value = 1; value <= DiceResultCount; ++value)
        {
            m_diceResults[static_cast<std::size_t>(value - 1)] =
                loadTextureIfPresent(
                    std::string(DicePath) +
                    "dice_result_" +
                    std::to_string(value) +
                    ".png",
                    loaded);
            allLoaded = allLoaded && loaded;
        }

        // ---------------------------------------------------------------------
        // Action Bar
        // ---------------------------------------------------------------------

        m_traversalActionBar = loadTextureIfPresent(
            std::string(ActionBarPath) +
            "traversal_action_bar.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_inventoryActionBar = loadTextureIfPresent(
            std::string(ActionBarPath) +
            "inventory_action_bar.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_combatActionBar = loadTextureIfPresent(
            std::string(ActionBarPath) +
            "combat_action_bar.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_loaded = allLoaded;

        m_portalActionBar = loadTextureIfPresent(
            std::string(ActionBarPath) +
            "portal_action_bar.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_defeatedActionBar = loadTextureIfPresent(
            std::string(ActionBarPath) +
            "defeated_action_bar.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_loaded = allLoaded;

        return m_loaded;
    }

    void MainScreenAssets::unload() noexcept
    {
        auto unloadTexture =
            [](Texture2D& texture)
            {
                if (isValidTexture(texture))
                {
                    UnloadTexture(texture);
                    texture = Texture2D{};
                }
            };

        unloadTexture(m_titleArt);
        unloadTexture(m_statsTitle);
        unloadTexture(m_gearTitle);

        unloadTexture(m_playerStatsWindowBackground);
        unloadTexture(m_gearWindowBackground);
        unloadTexture(m_diceRollTitle);
        unloadTexture(m_diceRollWindowBackground);
        unloadTexture(m_combatInfoWindowBackground);
        unloadTexture(m_lootTableWindowBackground);
        unloadTexture(m_lootTitle);
        unloadTexture(m_enemyStatsWindowBackground);
        unloadTexture(m_enemyStatsTitle);

        unloadTexture(m_atkIcon);
        unloadTexture(m_strIcon);
        unloadTexture(m_precIcon);
        unloadTexture(m_dmgIcon);
        unloadTexture(m_defIcon);
        unloadTexture(m_toughIcon);
        unloadTexture(m_hpIcon);

        for (Texture2D& texture : m_numbers)
        {
            unloadTexture(texture);
        }

        unloadTexture(m_healthPotionIcon);
        unloadTexture(m_ragePotionIcon);

        for (Texture2D& texture : m_healthPotionAmounts)
        {
            unloadTexture(texture);
        }

        for (Texture2D& texture : m_ragePotionAmounts)
        {
            unloadTexture(texture);
        }

        for (Texture2D& texture : m_weaponTiers)
        {
            unloadTexture(texture);
        }

        for (Texture2D& texture : m_armorTiers)
        {
            unloadTexture(texture);
        }

        unloadTexture(m_noAccessory);
        unloadTexture(m_magicSkull);
        unloadTexture(m_orcFang);
        unloadTexture(m_trollHeart);

        unloadTexture(m_wallTile);
        unloadTexture(m_floorTile);
        unloadTexture(m_playerTile);
        unloadTexture(m_chestTile);
        unloadTexture(m_chestOpenedTile);
        unloadTexture(m_goblinTile);
        unloadTexture(m_goblinDefeatedTile);
        unloadTexture(m_skeletonTile);
        unloadTexture(m_skeletonDefeatedTile);
        unloadTexture(m_orcTile);
        unloadTexture(m_orcDefeatedTile);
        unloadTexture(m_trollTile);
        unloadTexture(m_trollDefeatedTile);
        unloadTexture(m_dragonTile);
        unloadTexture(m_dragonDefeatedTile);
        unloadTexture(m_mapFrame);
        unloadTexture(m_portalTile);

        unloadTexture(m_traversalActionBar);
        unloadTexture(m_inventoryActionBar);
        unloadTexture(m_combatActionBar);
        unloadTexture(m_portalActionBar);
        unloadTexture(m_defeatedActionBar);

        for (Texture2D& texture : m_enemyVictory)
        {
            unloadTexture(texture);
        }

        for (Texture2D& texture : m_seesEnemies)
        {
            unloadTexture(texture);
        }

        for (Texture2D& texture : m_seesDefeatedEnemies)
        {
            unloadTexture(texture);
        }

        unloadTexture(m_seesChest);
        unloadTexture(m_seesDefeatedChest);
        unloadTexture(m_seesWall);
        unloadTexture(m_seesNothing);
        unloadTexture(m_portal);
        unloadTexture(m_seesPortal);

        unloadTexture(m_chestIdle);
        unloadTexture(m_chestDeath);

        for (Texture2D& texture : m_enemyIdle)
        {
            unloadTexture(texture);
        }

        for (Texture2D& texture : m_enemyHurt)
        {
            unloadTexture(texture);
        }

        for (Texture2D& texture : m_enemyAttack)
        {
            unloadTexture(texture);
        }

        for (Texture2D& texture : m_enemyDeath)
        {
            unloadTexture(texture);
        }

        unloadTexture(m_gameInstructions);
        unloadTexture(m_combatInstructions);
        unloadTexture(m_inventoryInstructions);
        unloadTexture(m_gameInstructionsBackground);
        unloadTexture(m_combatInstructionsBackground);
        unloadTexture(m_inventoryInstructionsBackground);
        unloadTexture(m_diceRolling);

        for (Texture2D& texture : m_diceResults)
        {
            unloadTexture(texture);
        }

        m_loaded = false;
    }

    bool MainScreenAssets::loaded() const noexcept
    {
        return m_loaded;
    }

    const Texture2D& MainScreenAssets::titleArt() const noexcept
    {
        return m_titleArt;
    }

    const Texture2D& MainScreenAssets::statsTitle() const noexcept
    {
        return m_statsTitle;
    }

    const Texture2D& MainScreenAssets::gearTitle() const noexcept
    {
        return m_gearTitle;
    }

    const Texture2D& MainScreenAssets::playerStatsWindowBackground() const noexcept
    {
        return m_playerStatsWindowBackground;
    }

    const Texture2D& MainScreenAssets::gearWindowBackground() const noexcept
    {
        return m_gearWindowBackground;
    }

    const Texture2D& MainScreenAssets::diceRollTitle() const noexcept
    {
        return m_diceRollTitle;
    }

    const Texture2D& MainScreenAssets::diceRollWindowBackground() const noexcept
    {
        return m_diceRollWindowBackground;
    }

    const Texture2D& MainScreenAssets::combatInfoWindowBackground() const noexcept
    {
        return m_combatInfoWindowBackground;
    }

    const Texture2D& MainScreenAssets::lootTableWindowBackground() const noexcept
    {
        return m_lootTableWindowBackground;
    }

    const Texture2D& MainScreenAssets::lootTitle() const noexcept
    {
        return m_lootTitle;
    }

    const Texture2D& MainScreenAssets::enemyStatsWindowBackground() const noexcept
    {
        return m_enemyStatsWindowBackground;
    }

    const Texture2D& MainScreenAssets::enemyStatsTitle() const noexcept
    {
        return m_enemyStatsTitle;
    }

    const Texture2D& MainScreenAssets::atkIcon() const noexcept
    {
        return m_atkIcon;
    }

    const Texture2D& MainScreenAssets::strIcon() const noexcept
    {
        return m_strIcon;
    }

    const Texture2D& MainScreenAssets::precIcon() const noexcept
    {
        return m_precIcon;
    }

    const Texture2D& MainScreenAssets::dmgIcon() const noexcept
    {
        return m_dmgIcon;
    }

    const Texture2D& MainScreenAssets::defIcon() const noexcept
    {
        return m_defIcon;
    }

    const Texture2D& MainScreenAssets::toughIcon() const noexcept
    {
        return m_toughIcon;
    }

    const Texture2D& MainScreenAssets::hpIcon() const noexcept
    {
        return m_hpIcon;
    }

    const Texture2D& MainScreenAssets::number(int value) const noexcept
    {
        if (value < 0 || value >= NumberAssetCount)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_numbers[static_cast<std::size_t>(value)];
    }

    const Texture2D& MainScreenAssets::healthPotionIcon() const noexcept
    {
        return m_healthPotionIcon;
    }

    const Texture2D& MainScreenAssets::ragePotionIcon() const noexcept
    {
        return m_ragePotionIcon;
    }

    const Texture2D& MainScreenAssets::healthPotionAmount(
        int amount) const noexcept
    {
        if (amount < 0 || amount >= PotionStateCount)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_healthPotionAmounts[
            static_cast<std::size_t>(amount)];
    }

    const Texture2D& MainScreenAssets::ragePotionAmount(
        int amount) const noexcept
    {
        if (amount < 0 || amount >= PotionStateCount)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_ragePotionAmounts[
            static_cast<std::size_t>(amount)];
    }

    const Texture2D& MainScreenAssets::weaponTier(int tier) const noexcept
    {
        if (tier < 1 || tier > GearTierCount)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_weaponTiers[
            static_cast<std::size_t>(tier - 1)];
    }

    const Texture2D& MainScreenAssets::armorTier(int tier) const noexcept
    {
        if (tier < 1 || tier > GearTierCount)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_armorTiers[
            static_cast<std::size_t>(tier - 1)];
    }

    const Texture2D& MainScreenAssets::noAccessory() const noexcept
    {
        return m_noAccessory;
    }

    const Texture2D& MainScreenAssets::magicSkull() const noexcept
    {
        return m_magicSkull;
    }

    const Texture2D& MainScreenAssets::orcFang() const noexcept
    {
        return m_orcFang;
    }

    const Texture2D& MainScreenAssets::trollHeart() const noexcept
    {
        return m_trollHeart;
    }

    const Texture2D& MainScreenAssets::wallTile() const noexcept
    {
        return m_wallTile;
    }

    const Texture2D& MainScreenAssets::floorTile() const noexcept
    {
        return m_floorTile;
    }

    const Texture2D& MainScreenAssets::playerTile() const noexcept
    {
        return m_playerTile;
    }

    const Texture2D& MainScreenAssets::chestTile() const noexcept
    {
        return m_chestTile;
    }

    const Texture2D& MainScreenAssets::chestOpenedTile() const noexcept
    {
        return m_chestOpenedTile;
    }

    const Texture2D& MainScreenAssets::goblinTile() const noexcept
    {
        return m_goblinTile;
    }

    const Texture2D& MainScreenAssets::goblinDefeatedTile() const noexcept
    {
        return m_goblinDefeatedTile;
    }

    const Texture2D& MainScreenAssets::skeletonTile() const noexcept
    {
        return m_skeletonTile;
    }

    const Texture2D& MainScreenAssets::skeletonDefeatedTile() const noexcept
    {
        return m_skeletonDefeatedTile;
    }

    const Texture2D& MainScreenAssets::orcTile() const noexcept
    {
        return m_orcTile;
    }

    const Texture2D& MainScreenAssets::orcDefeatedTile() const noexcept
    {
        return m_orcDefeatedTile;
    }

    const Texture2D& MainScreenAssets::trollTile() const noexcept
    {
        return m_trollTile;
    }

    const Texture2D& MainScreenAssets::trollDefeatedTile() const noexcept
    {
        return m_trollDefeatedTile;
    }

    const Texture2D& MainScreenAssets::dragonTile() const noexcept
    {
        return m_dragonTile;
    }

    const Texture2D& MainScreenAssets::dragonDefeatedTile() const noexcept
    {
        return m_dragonDefeatedTile;
    }

    const Texture2D& MainScreenAssets::mapFrame() const noexcept
    {
        return m_mapFrame;
    }

    const Texture2D& MainScreenAssets::traversalActionBar() const noexcept
    {
        return m_traversalActionBar;
    }

    const Texture2D& MainScreenAssets::inventoryActionBar() const noexcept
    {
        return m_inventoryActionBar;
    }

    const Texture2D& MainScreenAssets::combatActionBar() const noexcept
    {
        return m_combatActionBar;
    }

    const Texture2D& MainScreenAssets::seesEnemy(
        std::string_view type) const noexcept
    {
        const int index = enemyPovIndex(type);
        if (index < 0)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_seesEnemies[static_cast<std::size_t>(index)];
    }

    const Texture2D& MainScreenAssets::seesDefeatedEnemy(
        std::string_view type) const noexcept
    {
        const int index = enemyPovIndex(type);
        if (index < 0)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_seesDefeatedEnemies[static_cast<std::size_t>(index)];
    }

    const Texture2D& MainScreenAssets::seesChest() const noexcept
    {
        return m_seesChest;
    }

    const Texture2D& MainScreenAssets::seesDefeatedChest() const noexcept
    {
        return m_seesDefeatedChest;
    }

    const Texture2D& MainScreenAssets::seesWall() const noexcept
    {
        return m_seesWall;
    }

    const Texture2D& MainScreenAssets::seesNothing() const noexcept
    {
        return m_seesNothing;
    }

    const Texture2D& MainScreenAssets::chestIdle() const noexcept
    {
        return m_chestIdle;
    }

    const Texture2D& MainScreenAssets::chestDeath() const noexcept
    {
        return m_chestDeath;
    }

    const Texture2D& MainScreenAssets::enemyIdle(
        std::string_view type) const noexcept
    {
        const int index = enemyPovIndex(type);
        if (index < 0)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_enemyIdle[static_cast<std::size_t>(index)];
    }

    const Texture2D& MainScreenAssets::enemyHurt(
        std::string_view type) const noexcept
    {
        const int index = enemyPovIndex(type);
        if (index < 0)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_enemyHurt[static_cast<std::size_t>(index)];
    }

    const Texture2D& MainScreenAssets::enemyAttack(
        std::string_view type) const noexcept
    {
        const int index = enemyPovIndex(type);
        if (index < 0)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_enemyAttack[static_cast<std::size_t>(index)];
    }

    const Texture2D& MainScreenAssets::enemyDeath(
        std::string_view type) const noexcept
    {
        const int index = enemyPovIndex(type);
        if (index < 0)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_enemyDeath[static_cast<std::size_t>(index)];
    }

    const Texture2D& MainScreenAssets::gameInstructions() const noexcept
    {
        return m_gameInstructions;
    }

    const Texture2D& MainScreenAssets::combatInstructions() const noexcept
    {
        return m_combatInstructions;
    }

    const Texture2D& MainScreenAssets::inventoryInstructions() const noexcept
    {
        return m_inventoryInstructions;
    }

    const Texture2D& MainScreenAssets::gameInstructionsBackground() const noexcept
    {
        return m_gameInstructionsBackground;
    }

    const Texture2D& MainScreenAssets::combatInstructionsBackground() const noexcept
    {
        return m_combatInstructionsBackground;
    }

    const Texture2D& MainScreenAssets::inventoryInstructionsBackground() const noexcept
    {
        return m_inventoryInstructionsBackground;
    }

    const Texture2D& MainScreenAssets::diceRolling() const noexcept
    {
        return m_diceRolling;
    }

    const Texture2D& MainScreenAssets::diceResult(int value) const noexcept
    {
        if (value < 1 || value > DiceResultCount)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_diceResults[static_cast<std::size_t>(value - 1)];
    }

    const Texture2D& MainScreenAssets::portalTile() const noexcept
    {
        return m_portalTile;
    }

    const Texture2D& MainScreenAssets::portal() const noexcept
    {
        return m_portal;
    }

    const Texture2D& MainScreenAssets::seesPortal() const noexcept
    {
        return m_seesPortal;
    }

    const Texture2D& MainScreenAssets::enemyVictory(
        std::string_view type) const noexcept
    {
        const int index = enemyPovIndex(type);

        if (index < 0)
        {
            static const Texture2D Empty{};
            return Empty;
        }

        return m_enemyVictory[static_cast<std::size_t>(index)];
    }

    const Texture2D& MainScreenAssets::portalActionBar() const noexcept
    {
        return m_portalActionBar;
    }

    const Texture2D& MainScreenAssets::defeatedActionBar() const noexcept
    {
        return m_defeatedActionBar;
    }
}