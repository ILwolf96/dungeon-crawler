#include "rendering/Renderer.h"
#include "app/Game.h"

#include "raylib.h"

#include <cstddef>
#include <string>

namespace dungeon
{
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

    void Renderer::draw(const Game& game) const
    {
        ClearBackground(RAYWHITE);

        drawMainScreenLayout();
        drawMap(game);
        drawStatSlots();
        drawGearSlots();
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

        /*
            Temporary Map Frame.
            Later this entire frame will be replaced with the Map Frame PNG.
        */
        DrawRectangleRec(
            mapRender,
            DARKGRAY);

        // ---------------------------------------------------------------------
        // Visible map area
        //
        // 470x470 outer render area
        // 27px frame on every side
        // 416x416 inner visual map
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

        DrawRectangleRec(
            visibleMap,
            BLACK);

        // ---------------------------------------------------------------------
        // Temporary map rendering
        //
        // This remains so that the game is still visibly functional.
        // The future map PNG tiles will replace this section.
        // ---------------------------------------------------------------------

        const int renderedMapWidth =
            static_cast<int>(map.width()) * TemporaryMapTileSize;

        const int renderedMapHeight =
            static_cast<int>(map.height()) * TemporaryMapTileSize;

        // Center the currently loaded map inside the 416x416 visible area.
        const int mapOffsetX =
            visibleMapX +
            (VisibleMapWidth - renderedMapWidth) / 2;

        const int mapOffsetY =
            visibleMapY +
            (VisibleMapHeight - renderedMapHeight) / 2;

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

                const Rectangle tileRectangle = toRaylibRectangle(
                    tileX,
                    tileY,
                    TemporaryMapTileSize,
                    TemporaryMapTileSize);

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
                        LIGHTGRAY);
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

        const int playerCenterX =
            playerX +
            TemporaryMapTileSize / 2;

        const int playerCenterY =
            playerY +
            TemporaryMapTileSize / 2;

        DrawCircle(
            playerCenterX,
            ScreenHeight - playerCenterY,
            static_cast<float>(TemporaryMapTileSize / 3),
            MAROON);

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

            const int enemyCenterX =
                enemyX +
                TemporaryMapTileSize / 2;

            const int enemyCenterY =
                enemyY +
                TemporaryMapTileSize / 2;

            DrawCircle(
                enemyCenterX,
                ScreenHeight - enemyCenterY,
                static_cast<float>(TemporaryMapTileSize / 3),
                RED);

            const std::string enemyType(enemy->type());

            DrawText(
                enemyType.c_str(),
                enemyCenterX - MeasureText(
                    enemyType.c_str(),
                    6) / 2,
                ScreenHeight - enemyCenterY - 3,
                6,
                BLACK);
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

            const int chestCenterX =
                chestX +
                TemporaryMapTileSize / 2;

            const int chestCenterY =
                chestY +
                TemporaryMapTileSize / 2;

            const int chestSize =
                (TemporaryMapTileSize * 2) / 3;

            DrawRectangle(
                chestCenterX - chestSize / 2,
                ScreenHeight - chestCenterY - chestSize / 2,
                chestSize,
                chestSize,
                GOLD);

            DrawText(
                "Chest",
                chestCenterX - MeasureText(
                    "Chest",
                    6) / 2,
                ScreenHeight - chestCenterY - 3,
                6,
                BLACK);
        }

        // ---------------------------------------------------------------------
        // Temporary visible-map boundary.
        //
        // This will remain useful for verifying the 416x416 viewport.
        // ---------------------------------------------------------------------

        DrawRectangleLinesEx(
            visibleMap,
            1.0f,
            WHITE);
    }
}