#include "rendering/MainScreenAssets.h"

#include "raylib.h"

#include <array>
#include <string>

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

        m_goblinTile = loadTextureIfPresent(
            std::string(MapPath) + "goblin_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_skeletonTile = loadTextureIfPresent(
            std::string(MapPath) + "skeleton_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_orcTile = loadTextureIfPresent(
            std::string(MapPath) + "orc_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_trollTile = loadTextureIfPresent(
            std::string(MapPath) + "troll_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_dragonTile = loadTextureIfPresent(
            std::string(MapPath) + "dragon_tile.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_mapFrame = loadTextureIfPresent(
            std::string(MapPath) + "map_frame.png",
            loaded);
        allLoaded = allLoaded && loaded;

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
        unloadTexture(m_goblinTile);
        unloadTexture(m_skeletonTile);
        unloadTexture(m_orcTile);
        unloadTexture(m_trollTile);
        unloadTexture(m_dragonTile);
        unloadTexture(m_mapFrame);

        unloadTexture(m_traversalActionBar);
        unloadTexture(m_inventoryActionBar);

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

    const Texture2D& MainScreenAssets::goblinTile() const noexcept
    {
        return m_goblinTile;
    }

    const Texture2D& MainScreenAssets::skeletonTile() const noexcept
    {
        return m_skeletonTile;
    }

    const Texture2D& MainScreenAssets::orcTile() const noexcept
    {
        return m_orcTile;
    }

    const Texture2D& MainScreenAssets::trollTile() const noexcept
    {
        return m_trollTile;
    }

    const Texture2D& MainScreenAssets::dragonTile() const noexcept
    {
        return m_dragonTile;
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
}