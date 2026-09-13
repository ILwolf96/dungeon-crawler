#pragma once

#include "raylib.h"

#include <array>
#include <string>
#include <string_view>

namespace dungeon
{
    class MainScreenAssets
    {
    public:
        MainScreenAssets() = default;
        ~MainScreenAssets();

        MainScreenAssets(const MainScreenAssets&) = delete;
        MainScreenAssets& operator=(const MainScreenAssets&) = delete;

        bool load();
        void unload() noexcept;

        bool loaded() const noexcept;

        // ---------------------------------------------------------------------
        // Titles
        // ---------------------------------------------------------------------

        const Texture2D& titleArt() const noexcept;
        const Texture2D& statsTitle() const noexcept;
        const Texture2D& gearTitle() const noexcept;

        // Window and panel backgrounds / titles
        const Texture2D& playerStatsWindowBackground() const noexcept;
        const Texture2D& gearWindowBackground() const noexcept;
        const Texture2D& diceRollTitle() const noexcept;
        const Texture2D& diceRollWindowBackground() const noexcept;
        const Texture2D& combatInfoWindowBackground() const noexcept;
        const Texture2D& lootTableWindowBackground() const noexcept;
        const Texture2D& lootTitle() const noexcept;
        const Texture2D& enemyStatsWindowBackground() const noexcept;
        const Texture2D& enemyStatsTitle() const noexcept;

        // ---------------------------------------------------------------------
        // Stats
        // ---------------------------------------------------------------------

        const Texture2D& atkIcon() const noexcept;
        const Texture2D& strIcon() const noexcept;
        const Texture2D& precIcon() const noexcept;
        const Texture2D& dmgIcon() const noexcept;
        const Texture2D& defIcon() const noexcept;
        const Texture2D& toughIcon() const noexcept;
        const Texture2D& hpIcon() const noexcept;

        const Texture2D& number(int value) const noexcept;

        // ---------------------------------------------------------------------
        // Gear - Potions
        // ---------------------------------------------------------------------

        const Texture2D& healthPotionIcon() const noexcept;
        const Texture2D& ragePotionIcon() const noexcept;

        const Texture2D& healthPotionAmount(int amount) const noexcept;
        const Texture2D& ragePotionAmount(int amount) const noexcept;

        // ---------------------------------------------------------------------
        // Gear - Weapons / Armor
        // ---------------------------------------------------------------------

        const Texture2D& weaponTier(int tier) const noexcept;
        const Texture2D& armorTier(int tier) const noexcept;

        // ---------------------------------------------------------------------
        // Gear - Accessories
        // ---------------------------------------------------------------------

        const Texture2D& noAccessory() const noexcept;
        const Texture2D& magicSkull() const noexcept;
        const Texture2D& orcFang() const noexcept;
        const Texture2D& trollHeart() const noexcept;

        // ---------------------------------------------------------------------
        // Map
        // ---------------------------------------------------------------------

        const Texture2D& wallTile() const noexcept;
        const Texture2D& floorTile() const noexcept;
        const Texture2D& playerTile() const noexcept;
        const Texture2D& chestTile() const noexcept;
        const Texture2D& chestOpenedTile() const noexcept;
        const Texture2D& goblinTile() const noexcept;
        const Texture2D& goblinDefeatedTile() const noexcept;
        const Texture2D& skeletonTile() const noexcept;
        const Texture2D& skeletonDefeatedTile() const noexcept;
        const Texture2D& orcTile() const noexcept;
        const Texture2D& orcDefeatedTile() const noexcept;
        const Texture2D& trollTile() const noexcept;
        const Texture2D& trollDefeatedTile() const noexcept;
        const Texture2D& dragonTile() const noexcept;
        const Texture2D& dragonDefeatedTile() const noexcept;
        const Texture2D& mapFrame() const noexcept;
        const Texture2D& portalTile() const noexcept;
        const Texture2D& portal() const noexcept;

        // ---------------------------------------------------------------------
        // Main Screen Action Bar
        // ---------------------------------------------------------------------

        const Texture2D& traversalActionBar() const noexcept;
        const Texture2D& inventoryActionBar() const noexcept;
        const Texture2D& combatActionBar() const noexcept;
        const Texture2D& portalActionBar() const noexcept;
        const Texture2D& defeatedActionBar() const noexcept;

        // ---------------------------------------------------------------------
        // POV - Traversal
        // ---------------------------------------------------------------------

        const Texture2D& seesEnemy(std::string_view type) const noexcept;
        const Texture2D& seesDefeatedEnemy(std::string_view type) const noexcept;
        const Texture2D& seesChest() const noexcept;
        const Texture2D& seesDefeatedChest() const noexcept;
        const Texture2D& seesWall() const noexcept;
        const Texture2D& seesNothing() const noexcept;
        const Texture2D& seesPortal() const noexcept;

        // ---------------------------------------------------------------------
        // POV - Combat
        // ---------------------------------------------------------------------


        const Texture2D& chestIdle() const noexcept;
        const Texture2D& chestDeath() const noexcept;

        const Texture2D& enemyIdle(std::string_view type) const noexcept;
        const Texture2D& enemyHurt(std::string_view type) const noexcept;
        const Texture2D& enemyAttack(std::string_view type) const noexcept;
        const Texture2D& enemyDeath(std::string_view type) const noexcept;
        const Texture2D& enemyVictory(std::string_view type) const noexcept;

        // Instructions
        const Texture2D& gameInstructions() const noexcept;
        const Texture2D& combatInstructions() const noexcept;
        const Texture2D& inventoryInstructions() const noexcept;
        const Texture2D& gameInstructionsBackground() const noexcept;
        const Texture2D& combatInstructionsBackground() const noexcept;
        const Texture2D& inventoryInstructionsBackground() const noexcept;

        // Dice presentation
        const Texture2D& diceRolling() const noexcept;
        const Texture2D& diceResult(int value) const noexcept;

    private:
        static constexpr int NumberAssetCount = 17;
        static constexpr int PotionStateCount = 4;
        static constexpr int GearTierCount = 6;
        static constexpr int EnemyPovTypeCount = 5;
        static constexpr int DiceResultCount = 6;

        static Texture2D emptyTexture() noexcept;
        static Texture2D loadTextureIfPresent(const std::string& path, bool& loadedSuccessfully);

        static bool isValidTexture(const Texture2D& texture) noexcept;
        static int enemyPovIndex(std::string_view type) noexcept;

        Texture2D m_titleArt{};
        Texture2D m_statsTitle{};
        Texture2D m_gearTitle{};

        Texture2D m_playerStatsWindowBackground{};
        Texture2D m_gearWindowBackground{};
        Texture2D m_diceRollTitle{};
        Texture2D m_diceRollWindowBackground{};
        Texture2D m_combatInfoWindowBackground{};
        Texture2D m_lootTableWindowBackground{};
        Texture2D m_lootTitle{};
        Texture2D m_enemyStatsWindowBackground{};
        Texture2D m_enemyStatsTitle{};

        Texture2D m_atkIcon{};
        Texture2D m_strIcon{};
        Texture2D m_precIcon{};
        Texture2D m_dmgIcon{};
        Texture2D m_defIcon{};
        Texture2D m_toughIcon{};
        Texture2D m_hpIcon{};

        std::array<Texture2D, NumberAssetCount> m_numbers{};

        Texture2D m_healthPotionIcon{};
        Texture2D m_ragePotionIcon{};

        std::array<Texture2D, PotionStateCount> m_healthPotionAmounts{};
        std::array<Texture2D, PotionStateCount> m_ragePotionAmounts{};

        std::array<Texture2D, GearTierCount> m_weaponTiers{};
        std::array<Texture2D, GearTierCount> m_armorTiers{};

        Texture2D m_noAccessory{};
        Texture2D m_magicSkull{};
        Texture2D m_orcFang{};
        Texture2D m_trollHeart{};

        Texture2D m_wallTile{};
        Texture2D m_floorTile{};
        Texture2D m_playerTile{};
        Texture2D m_chestTile{};
        Texture2D m_chestOpenedTile{};
        Texture2D m_goblinTile{};
        Texture2D m_goblinDefeatedTile{};
        Texture2D m_skeletonTile{};
        Texture2D m_skeletonDefeatedTile{};
        Texture2D m_orcTile{};
        Texture2D m_orcDefeatedTile{};
        Texture2D m_trollTile{};
        Texture2D m_trollDefeatedTile{};
        Texture2D m_dragonTile{};
        Texture2D m_dragonDefeatedTile{};
        Texture2D m_mapFrame{};
        Texture2D m_portalTile{};


        Texture2D m_traversalActionBar{};
        Texture2D m_inventoryActionBar{};
        Texture2D m_combatActionBar{};
        Texture2D m_portalActionBar{};
        Texture2D m_defeatedActionBar{};

        std::array<Texture2D, EnemyPovTypeCount> m_seesEnemies{};
        std::array<Texture2D, EnemyPovTypeCount> m_seesDefeatedEnemies{};
        Texture2D m_seesChest{};
        Texture2D m_seesDefeatedChest{};
        Texture2D m_seesWall{};
        Texture2D m_seesNothing{};
        Texture2D m_portal{};
        Texture2D m_seesPortal{};


        Texture2D m_chestIdle;
        Texture2D m_chestDeath;

        std::array<Texture2D, EnemyPovTypeCount> m_enemyIdle{};
        std::array<Texture2D, EnemyPovTypeCount> m_enemyHurt{};
        std::array<Texture2D, EnemyPovTypeCount> m_enemyAttack{};
        std::array<Texture2D, EnemyPovTypeCount> m_enemyDeath{};
        std::array<Texture2D, EnemyPovTypeCount> m_enemyVictory{};

        Texture2D m_gameInstructions{};
        Texture2D m_combatInstructions{};
        Texture2D m_inventoryInstructions{};
        Texture2D m_gameInstructionsBackground{};
        Texture2D m_combatInstructionsBackground{};
        Texture2D m_inventoryInstructionsBackground{};

        Texture2D m_diceRolling{};
        std::array<Texture2D, DiceResultCount> m_diceResults{};

        bool m_loaded{ false };
    };
}