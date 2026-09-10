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
        const Texture2D& goblinTile() const noexcept;
        const Texture2D& skeletonTile() const noexcept;
        const Texture2D& orcTile() const noexcept;
        const Texture2D& trollTile() const noexcept;
        const Texture2D& dragonTile() const noexcept;
        const Texture2D& mapFrame() const noexcept;

        // ---------------------------------------------------------------------
        // Main Screen Action Bar
        // ---------------------------------------------------------------------

        const Texture2D& traversalActionBar() const noexcept;
        const Texture2D& inventoryActionBar() const noexcept;
        const Texture2D& combatActionBar() const noexcept;

        // ---------------------------------------------------------------------
        // POV - Traversal
        // ---------------------------------------------------------------------

        const Texture2D& seesEnemy(std::string_view type) const noexcept;
        const Texture2D& seesDefeatedEnemy(std::string_view type) const noexcept;
        const Texture2D& seesChest() const noexcept;
        const Texture2D& seesDefeatedChest() const noexcept;

        // ---------------------------------------------------------------------
        // POV - Combat
        // ---------------------------------------------------------------------

        const Texture2D& enemyIdle(std::string_view type) const noexcept;
        const Texture2D& enemyHurt(std::string_view type) const noexcept;
        const Texture2D& enemyAttack(std::string_view type) const noexcept;
        const Texture2D& enemyDeath(std::string_view type) const noexcept;

    private:
        static constexpr int NumberAssetCount = 17;
        static constexpr int PotionStateCount = 4;
        static constexpr int GearTierCount = 6;
        static constexpr int EnemyPovTypeCount = 5;

        static Texture2D emptyTexture() noexcept;
        static Texture2D loadTextureIfPresent(
            const std::string& path,
            bool& loadedSuccessfully);

        static bool isValidTexture(const Texture2D& texture) noexcept;
        static int enemyPovIndex(std::string_view type) noexcept;

        Texture2D m_titleArt{};
        Texture2D m_statsTitle{};
        Texture2D m_gearTitle{};

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
        Texture2D m_goblinTile{};
        Texture2D m_skeletonTile{};
        Texture2D m_orcTile{};
        Texture2D m_trollTile{};
        Texture2D m_dragonTile{};
        Texture2D m_mapFrame{};

        Texture2D m_traversalActionBar{};
        Texture2D m_inventoryActionBar{};
        Texture2D m_combatActionBar{};

        std::array<Texture2D, EnemyPovTypeCount> m_seesEnemies{};
        std::array<Texture2D, EnemyPovTypeCount> m_seesDefeatedEnemies{};
        Texture2D m_seesChest{};
        Texture2D m_seesDefeatedChest{};

        std::array<Texture2D, EnemyPovTypeCount> m_enemyIdle{};
        std::array<Texture2D, EnemyPovTypeCount> m_enemyHurt{};
        std::array<Texture2D, EnemyPovTypeCount> m_enemyAttack{};
        std::array<Texture2D, EnemyPovTypeCount> m_enemyDeath{};

        bool m_loaded{ false };
    };
}
