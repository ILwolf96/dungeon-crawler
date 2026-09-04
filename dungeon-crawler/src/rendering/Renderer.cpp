#include "rendering/Renderer.h"
#include "app/Game.h"

#include "raylib.h"

#include "entities/Chest.h"
#include "entities/Dragon.h"
#include "entities/Goblin.h"
#include "entities/Orc.h"
#include "entities/Skeleton.h"
#include "entities/Troll.h"
#include "inventory/HealthPotion.h"
#include "inventory/RagePotion.h"

#include <cstddef>
#include <string>
#include <string_view>

namespace dungeon
{
    namespace
    {
        const Texture2D* mapTextureForTile(
            const MainScreenAssets& assets,
            char tile)
        {
            if (tile == '#')
            {
                return &assets.wallTile();
            }

            return &assets.floorTile();
        }
    }

    int Renderer::toRaylibY(
        int bottomLeftY,
        int height) noexcept
    {
        return ScreenHeight - bottomLeftY - height;
    }

    Rectangle Renderer::toRaylibRectangle(
        int x,
        int y,
        int width,
        int height) noexcept
    {
        return Rectangle{
            static_cast<float>(x),
            static_cast<float>(toRaylibY(y, height)),
            static_cast<float>(width),
            static_cast<float>(height)
        };
    }

    bool Renderer::loadMainScreenAssets()
    {
        return m_mainScreenAssets.load();
    }

    void Renderer::drawTexture(
        const Texture2D& texture,
        int x,
        int y,
        int width,
        int height) const
    {
        if (texture.id == 0)
        {
            return;
        }

        const Rectangle destination =
            toRaylibRectangle(
                x,
                y,
                width,
                height);

        const Rectangle source{
            0.0f,
            0.0f,
            static_cast<float>(texture.width),
            static_cast<float>(texture.height)
        };

        DrawTexturePro(
            texture,
            source,
            destination,
            Vector2{ 0.0f, 0.0f },
            0.0f,
            WHITE);
    }

    void Renderer::draw(const Game& game) const
    {
        ClearBackground(RAYWHITE);

        drawMainScreenLayout();
        drawMap(game);

        // ---------------------------------------------------------------------
        // Temporary fallback slots
        //
        // These remain underneath the PNG assets so that the established
        // positions are still visible whenever an asset is missing.
        // ---------------------------------------------------------------------

        drawStatSlots();
        drawGearSlots();

        // ---------------------------------------------------------------------
        // PNG assets
        // ---------------------------------------------------------------------

        drawTitleAssets();
        drawStatAssets(game);
        drawGearAssets(game);
        drawActionBar();
    }

    void Renderer::drawMainScreenLayout() const
    {
        // ---------------------------------------------------------------------
        // Title Art
        //
        // Temporary placeholder.
        // This will later be replaced by the Title Art PNG.
        // ---------------------------------------------------------------------

        const Rectangle titleArt = toRaylibRectangle(
            TitleArtX,
            TitleArtY,
            TitleArtWidth,
            TitleArtHeight);

        DrawRectangleRec(
            titleArt,
            LIGHTGRAY);

        DrawRectangleLinesEx(
            titleArt,
            1.0f,
            DARKGRAY);

        // ---------------------------------------------------------------------
        // Stats Title
        //
        // Temporary placeholder.
        // This will later be replaced by the Stats Title PNG.
        // ---------------------------------------------------------------------

        const Rectangle statsTitle = toRaylibRectangle(
            StatsTitleX,
            StatsTitleY,
            StatsTitleWidth,
            StatsTitleHeight);

        DrawRectangleRec(
            statsTitle,
            LIGHTGRAY);

        DrawRectangleLinesEx(
            statsTitle,
            1.0f,
            DARKGRAY);

        // ---------------------------------------------------------------------
        // POV
        //
        // Temporary placeholder.
        // This will later be replaced by the POV PNG.
        // ---------------------------------------------------------------------

        const Rectangle povRender = toRaylibRectangle(
            PovRenderX,
            PovRenderY,
            PovRenderWidth,
            PovRenderHeight);

        DrawRectangleRec(
            povRender,
            RAYWHITE);

        DrawRectangleLinesEx(
            povRender,
            1.0f,
            DARKGRAY);

        // ---------------------------------------------------------------------
        // Stats Window
        //
        // Temporary placeholder background.
        // ---------------------------------------------------------------------

        const Rectangle statsWindow = toRaylibRectangle(
            StatsWindowX,
            StatsWindowY,
            StatsWindowWidth,
            StatsWindowHeight);

        DrawRectangleRec(
            statsWindow,
            RAYWHITE);

        DrawRectangleLinesEx(
            statsWindow,
            1.0f,
            DARKGRAY);

        // ---------------------------------------------------------------------
        // Gear Title
        //
        // Temporary placeholder.
        // This will later be replaced by the Gear Title PNG.
        // ---------------------------------------------------------------------

        const Rectangle gearTitle = toRaylibRectangle(
            GearTitleX,
            GearTitleY,
            GearTitleWidth,
            GearTitleHeight);

        DrawRectangleRec(
            gearTitle,
            LIGHTGRAY);

        DrawRectangleLinesEx(
            gearTitle,
            1.0f,
            DARKGRAY);

        // ---------------------------------------------------------------------
        // Gear Window
        //
        // Temporary placeholder background.
        // ---------------------------------------------------------------------

        const Rectangle gearWindow = toRaylibRectangle(
            GearWindowX,
            GearWindowY,
            GearWindowWidth,
            GearWindowHeight);

        DrawRectangleRec(
            gearWindow,
            RAYWHITE);

        DrawRectangleLinesEx(
            gearWindow,
            1.0f,
            DARKGRAY);

        // ---------------------------------------------------------------------
        // Action Bar
        //
        // Temporary placeholder.
        // This will later be replaced by the Action Bar PNG.
        // ---------------------------------------------------------------------

        const Rectangle actionBar = toRaylibRectangle(
            ActionBarX,
            ActionBarY,
            ActionBarWidth,
            ActionBarHeight);

        DrawRectangleRec(
            actionBar,
            LIGHTGRAY);

        DrawRectangleLinesEx(
            actionBar,
            1.0f,
            DARKGRAY);
    }

    void Renderer::drawStatSlots() const
    {
        // ---------------------------------------------------------------------
        // Temporary Stat PNG placeholders
        //
        // Every slot is exactly 32x32.
        //
        // The final Stat Icon / Number PNGs will be drawn in these exact
        // positions. These temporary rectangles can then be commented out.
        // ---------------------------------------------------------------------

        const int iconX = StatIconX;
        const int numberX = StatNumberX;

        const int statYPositions[] =
        {
            StatAtkY,
            StatStrY,
            StatPrecY,
            StatDmgY,
            StatDefY,
            StatToughY,
            StatHpY
        };

        for (const int y : statYPositions)
        {
            const Rectangle icon = toRaylibRectangle(
                iconX,
                y,
                StatIconSize,
                StatIconSize);

            const Rectangle number = toRaylibRectangle(
                numberX,
                y,
                StatNumberSize,
                StatNumberSize);

            DrawRectangleLinesEx(
                icon,
                1.0f,
                GRAY);

            DrawRectangleLinesEx(
                number,
                1.0f,
                GRAY);
        }
    }

    void Renderer::drawGearSlots() const
    {
        // ---------------------------------------------------------------------
        // Temporary Gear PNG placeholders
        //
        // Every Gear Icon / Number is exactly 48x48.
        //
        // The final PNGs will be drawn in these exact positions.
        // ---------------------------------------------------------------------

        // Health Potion
        DrawRectangleLinesEx(
            toRaylibRectangle(
                PotionIconX,
                HealthPotionY,
                GearIconSize,
                GearIconSize),
            1.0f,
            GRAY);

        DrawRectangleLinesEx(
            toRaylibRectangle(
                PotionNumberX,
                HealthPotionY,
                GearIconSize,
                GearIconSize),
            1.0f,
            GRAY);

        // Rage Potion
        DrawRectangleLinesEx(
            toRaylibRectangle(
                PotionIconX,
                RagePotionY,
                GearIconSize,
                GearIconSize),
            1.0f,
            GRAY);

        DrawRectangleLinesEx(
            toRaylibRectangle(
                PotionNumberX,
                RagePotionY,
                GearIconSize,
                GearIconSize),
            1.0f,
            GRAY);

        // Weapon
        DrawRectangleLinesEx(
            toRaylibRectangle(
                WeaponIconX,
                WeaponArmorY,
                GearIconSize,
                GearIconSize),
            1.0f,
            GRAY);

        // Armor
        DrawRectangleLinesEx(
            toRaylibRectangle(
                ArmorIconX,
                WeaponArmorY,
                GearIconSize,
                GearIconSize),
            1.0f,
            GRAY);

        // Tier 1 Accessory
        DrawRectangleLinesEx(
            toRaylibRectangle(
                Tier1AccessoryX,
                AccessoriesY,
                GearIconSize,
                GearIconSize),
            1.0f,
            GRAY);

        // Tier 2 Accessory
        DrawRectangleLinesEx(
            toRaylibRectangle(
                Tier2AccessoryX,
                AccessoriesY,
                GearIconSize,
                GearIconSize),
            1.0f,
            GRAY);

        // Tier 3 Accessory
        DrawRectangleLinesEx(
            toRaylibRectangle(
                Tier3AccessoryX,
                AccessoriesY,
                GearIconSize,
                GearIconSize),
            1.0f,
            GRAY);
    }

    void Renderer::drawTitleAssets() const
    {
        drawTexture(
            m_mainScreenAssets.titleArt(),
            TitleArtX,
            TitleArtY,
            TitleArtWidth,
            TitleArtHeight);

        drawTexture(
            m_mainScreenAssets.statsTitle(),
            StatsTitleX,
            StatsTitleY,
            StatsTitleWidth,
            StatsTitleHeight);

        drawTexture(
            m_mainScreenAssets.gearTitle(),
            GearTitleX,
            GearTitleY,
            GearTitleWidth,
            GearTitleHeight);
    }

    void Renderer::drawStatAssets(const Game& game) const
    {
        const CombatStats stats =
            game.player().combatStats();

        const int attack =
            stats.attacks;

        const int strength =
            stats.strength;

        const int precision =
            stats.precision;

        const int damage =
            game.player().weaponDamage();

        const int defense =
            stats.defense;

        const int toughness =
            stats.toughness;

        const int hp =
            game.player().currentHp();

        // -------------------------------------------------------------------------
        // Static Stat Icons
        // -------------------------------------------------------------------------

        drawTexture(
            m_mainScreenAssets.atkIcon(),
            StatIconX,
            StatAtkY,
            StatIconSize,
            StatIconSize);

        drawTexture(
            m_mainScreenAssets.strIcon(),
            StatIconX,
            StatStrY,
            StatIconSize,
            StatIconSize);

        drawTexture(
            m_mainScreenAssets.precIcon(),
            StatIconX,
            StatPrecY,
            StatIconSize,
            StatIconSize);

        drawTexture(
            m_mainScreenAssets.dmgIcon(),
            StatIconX,
            StatDmgY,
            StatIconSize,
            StatIconSize);

        drawTexture(
            m_mainScreenAssets.defIcon(),
            StatIconX,
            StatDefY,
            StatIconSize,
            StatIconSize);

        drawTexture(
            m_mainScreenAssets.toughIcon(),
            StatIconX,
            StatToughY,
            StatIconSize,
            StatIconSize);

        drawTexture(
            m_mainScreenAssets.hpIcon(),
            StatIconX,
            StatHpY,
            StatIconSize,
            StatIconSize);

        // -------------------------------------------------------------------------
        // Dynamic Number PNGs
        // -------------------------------------------------------------------------

        drawTexture(
            m_mainScreenAssets.number(attack),
            StatNumberX,
            StatAtkY,
            StatNumberSize,
            StatNumberSize);

        drawTexture(
            m_mainScreenAssets.number(strength),
            StatNumberX,
            StatStrY,
            StatNumberSize,
            StatNumberSize);

        drawTexture(
            m_mainScreenAssets.number(precision),
            StatNumberX,
            StatPrecY,
            StatNumberSize,
            StatNumberSize);

        drawTexture(
            m_mainScreenAssets.number(damage),
            StatNumberX,
            StatDmgY,
            StatNumberSize,
            StatNumberSize);

        drawTexture(
            m_mainScreenAssets.number(defense),
            StatNumberX,
            StatDefY,
            StatNumberSize,
            StatNumberSize);

        drawTexture(
            m_mainScreenAssets.number(toughness),
            StatNumberX,
            StatToughY,
            StatNumberSize,
            StatNumberSize);

        drawTexture(
            m_mainScreenAssets.number(hp),
            StatNumberX,
            StatHpY,
            StatNumberSize,
            StatNumberSize);
    }

    void Renderer::drawGearAssets(const Game& game) const
    {
        const Player& player =
            game.player();

        const Inventory& inventory =
            player.inventory();

        // -------------------------------------------------------------------------
        // Potion Icons
        // -------------------------------------------------------------------------

        drawTexture(
            m_mainScreenAssets.healthPotionIcon(),
            PotionIconX,
            HealthPotionY,
            GearIconSize,
            GearIconSize);

        drawTexture(
            m_mainScreenAssets.ragePotionIcon(),
            PotionIconX,
            RagePotionY,
            GearIconSize,
            GearIconSize);

        // -------------------------------------------------------------------------
        // Potion Amounts
        // -------------------------------------------------------------------------

        const int healthAmount =
            static_cast<int>(
                inventory.amount(HealthPotion{}));

        const int rageAmount =
            static_cast<int>(
                inventory.amount(RagePotion{}));

        drawTexture(
            m_mainScreenAssets.healthPotionAmount(
                healthAmount),
            PotionNumberX,
            HealthPotionY,
            GearIconSize,
            GearIconSize);

        drawTexture(
            m_mainScreenAssets.ragePotionAmount(
                rageAmount),
            PotionNumberX,
            RagePotionY,
            GearIconSize,
            GearIconSize);

        // -------------------------------------------------------------------------
        // Weapon
        // -------------------------------------------------------------------------

        const Weapon* weapon =
            player.equipment().weapon();

        if (weapon != nullptr)
        {
            drawTexture(
                m_mainScreenAssets.weaponTier(
                    weapon->tier()),
                WeaponIconX,
                WeaponArmorY,
                GearIconSize,
                GearIconSize);
        }

        // -------------------------------------------------------------------------
        // Armor
        // -------------------------------------------------------------------------

        const Armor* armor =
            player.equipment().armor();

        if (armor != nullptr)
        {
            drawTexture(
                m_mainScreenAssets.armorTier(
                    armor->tier()),
                ArmorIconX,
                WeaponArmorY,
                GearIconSize,
                GearIconSize);
        }

        // -------------------------------------------------------------------------
        // Accessories
        // -------------------------------------------------------------------------

        const auto& accessories =
            player.equipment().accessories();

        bool hasMagicSkull = false;
        bool hasOrcFang = false;
        bool hasTrollHeart = false;

        for (const auto& accessory : accessories)
        {
            const std::string_view name =
                accessory->name();

            if (name == "Magic Skull")
            {
                hasMagicSkull = true;
            }
            else if (name == "Orc Fang")
            {
                hasOrcFang = true;
            }
            else if (name == "Troll's Heart")
            {
                hasTrollHeart = true;
            }
        }

        drawTexture(
            hasMagicSkull
            ? m_mainScreenAssets.magicSkull()
            : m_mainScreenAssets.noAccessory(),
            Tier1AccessoryX,
            AccessoriesY,
            GearIconSize,
            GearIconSize);

        drawTexture(
            hasOrcFang
            ? m_mainScreenAssets.orcFang()
            : m_mainScreenAssets.noAccessory(),
            Tier2AccessoryX,
            AccessoriesY,
            GearIconSize,
            GearIconSize);

        drawTexture(
            hasTrollHeart
            ? m_mainScreenAssets.trollHeart()
            : m_mainScreenAssets.noAccessory(),
            Tier3AccessoryX,
            AccessoriesY,
            GearIconSize,
            GearIconSize);
    }

    void Renderer::drawActionBar() const
    {
        drawTexture(
            m_mainScreenAssets.traversalActionBar(),
            ActionBarX,
            ActionBarY,
            ActionBarWidth,
            ActionBarHeight);
    }

    void Renderer::drawMap(const Game& game) const
    {
        const Map& map = game.map();

        // ---------------------------------------------------------------------
        // Map Render area
        // ---------------------------------------------------------------------

        const Rectangle mapRender = toRaylibRectangle(
            MapRenderX,
            MapRenderY,
            MapRenderWidth,
            MapRenderHeight);

        // Temporary frame fallback.
        DrawRectangleRec(
            mapRender,
            DARKGRAY);

        // ---------------------------------------------------------------------
        // Visible map area
        // ---------------------------------------------------------------------

        const int visibleMapX =
            MapRenderX + MapFrameThickness;

        const int visibleMapY =
            MapRenderY + MapFrameThickness;

        const Rectangle visibleMap = toRaylibRectangle(
            visibleMapX,
            visibleMapY,
            VisibleMapWidth,
            VisibleMapHeight);

        // Temporary floor/background fallback.
        DrawRectangleRec(
            visibleMap,
            BLACK);

        // ---------------------------------------------------------------------
        // Map dimensions
        // ---------------------------------------------------------------------

        const int renderedMapWidth =
            static_cast<int>(map.width()) * TemporaryMapTileSize;

        const int renderedMapHeight =
            static_cast<int>(map.height()) * TemporaryMapTileSize;

        const int mapOffsetX =
            visibleMapX +
            (VisibleMapWidth - renderedMapWidth) / 2;

        const int mapOffsetY =
            visibleMapY +
            (VisibleMapHeight - renderedMapHeight) / 2;

        // ---------------------------------------------------------------------
        // Map tiles
        // ---------------------------------------------------------------------

        for (std::size_t y = 0; y < map.height(); ++y)
        {
            for (std::size_t x = 0; x < map.width(); ++x)
            {
                const char tile = map.tileAt(
                    static_cast<int>(x),
                    static_cast<int>(y));

                const int tileX =
                    mapOffsetX +
                    static_cast<int>(x) * TemporaryMapTileSize;

                const int tileY =
                    mapOffsetY +
                    renderedMapHeight -
                    TemporaryMapTileSize -
                    static_cast<int>(y) * TemporaryMapTileSize;

                const Rectangle tileRectangle =
                    toRaylibRectangle(
                        tileX,
                        tileY,
                        TemporaryMapTileSize,
                        TemporaryMapTileSize);

                const Texture2D* texture =
                    mapTextureForTile(
                        m_mainScreenAssets,
                        tile);

                // -------------------------------------------------------------
                // PNG version
                // -------------------------------------------------------------

                if (texture != nullptr && texture->id != 0)
                {
                    drawTexture(
                        *texture,
                        tileX,
                        tileY,
                        TemporaryMapTileSize,
                        TemporaryMapTileSize);

                    continue;
                }

                // -------------------------------------------------------------
                // Temporary fallback version
                // -------------------------------------------------------------

                if (tile == '#')
                {
                    DrawRectangleRec(
                        tileRectangle,
                        DARKGRAY);
                }
                else
                {
                    DrawRectangleRec(
                        tileRectangle,
                        GRAY);
                }
            }
        }

        // ---------------------------------------------------------------------
        // Player
        // ---------------------------------------------------------------------

        const Player& player = game.player();

        const int playerX =
            mapOffsetX +
            player.x() * TemporaryMapTileSize;

        const int playerY =
            mapOffsetY +
            renderedMapHeight -
            TemporaryMapTileSize -
            player.y() * TemporaryMapTileSize;

        const Texture2D& playerTexture =
            m_mainScreenAssets.playerTile();

        if (playerTexture.id != 0)
        {
            drawTexture(
                playerTexture,
                playerX,
                playerY,
                TemporaryMapTileSize,
                TemporaryMapTileSize);
        }
        else
        {
            DrawRectangle(
                playerX,
                toRaylibY(
                    playerY,
                    TemporaryMapTileSize),
                TemporaryMapTileSize,
                TemporaryMapTileSize,
                BLUE);
        }

        // ---------------------------------------------------------------------
        // Enemies
        // ---------------------------------------------------------------------

        const auto& enemies = game.enemies();

        for (const auto& enemy : enemies)
        {
            const int enemyX =
                mapOffsetX +
                enemy->x() * TemporaryMapTileSize;

            const int enemyY =
                mapOffsetY +
                renderedMapHeight -
                TemporaryMapTileSize -
                enemy->y() * TemporaryMapTileSize;

            const std::string_view type = enemy->type();

            const Texture2D* enemyTexture =
                &m_mainScreenAssets.goblinTile();

            if (type == "Skeleton")
            {
                enemyTexture =
                    &m_mainScreenAssets.skeletonTile();
            }
            else if (type == "Orc")
            {
                enemyTexture =
                    &m_mainScreenAssets.orcTile();
            }
            else if (type == "Troll")
            {
                enemyTexture =
                    &m_mainScreenAssets.trollTile();
            }
            else if (type == "Dragon")
            {
                enemyTexture =
                    &m_mainScreenAssets.dragonTile();
            }

            if (enemyTexture->id != 0)
            {
                drawTexture(
                    *enemyTexture,
                    enemyX,
                    enemyY,
                    TemporaryMapTileSize,
                    TemporaryMapTileSize);
            }
            else
            {
                DrawRectangle(
                    enemyX,
                    toRaylibY(
                        enemyY,
                        TemporaryMapTileSize),
                    TemporaryMapTileSize,
                    TemporaryMapTileSize,
                    RED);
            }
        }

        // ---------------------------------------------------------------------
        // Chests
        // ---------------------------------------------------------------------

        const auto& chests = game.chests();

        for (const auto& chest : chests)
        {
            const int chestX =
                mapOffsetX +
                chest->x() * TemporaryMapTileSize;

            const int chestY =
                mapOffsetY +
                renderedMapHeight -
                TemporaryMapTileSize -
                chest->y() * TemporaryMapTileSize;

            const Texture2D& chestTexture =
                m_mainScreenAssets.chestTile();

            if (chestTexture.id != 0)
            {
                drawTexture(
                    chestTexture,
                    chestX,
                    chestY,
                    TemporaryMapTileSize,
                    TemporaryMapTileSize);
            }
            else
            {
                DrawRectangle(
                    chestX,
                    toRaylibY(
                        chestY,
                        TemporaryMapTileSize),
                    TemporaryMapTileSize,
                    TemporaryMapTileSize,
                    GOLD);
            }
        }

        // ---------------------------------------------------------------------
        // Map Frame PNG
        // ---------------------------------------------------------------------

        const Texture2D& frameTexture =
            m_mainScreenAssets.mapFrame();

        if (frameTexture.id != 0)
        {
            drawTexture(
                frameTexture,
                MapRenderX,
                MapRenderY,
                MapRenderWidth,
                MapRenderHeight);
        }
        else
        {
            // Keep the temporary frame visible if the PNG isn't available.
            DrawRectangleLinesEx(
                mapRender,
                static_cast<float>(MapFrameThickness),
                DARKGRAY);
        }
    }
}