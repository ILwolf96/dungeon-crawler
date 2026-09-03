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

    void Renderer::draw(const Game& game) const
    {
        ClearBackground(RAYWHITE);

        drawMainScreenLayout();
        drawMap(game);
    }

    void Renderer::drawMainScreenLayout() const
    {
        // ---------------------------------------------------------------------
        // Title Art
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

    void Renderer::drawMap(const Game& game) const
    {
        const Map& map = game.map();

        // ---------------------------------------------------------------------
        // Map Render zone
        // ---------------------------------------------------------------------

        const Rectangle mapRender = toRaylibRectangle(
            MapRenderX,
            MapRenderY,
            MapRenderWidth,
            MapRenderHeight);

        DrawRectangleRec(
            mapRender,
            DARKGRAY);

        // ---------------------------------------------------------------------
        // 27 px Map Frame
        //
        // The actual visible map is the 416x416 area inside the frame.
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
        // 16x16 is being used only to establish the new map scale.
        // The map's actual PNG tiles will replace this later.
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

                /*
                    The configuration map is currently indexed from its
                    top row downward.

                    Our UI layout uses a bottom-left coordinate system, so
                    the map row is converted accordingly.
                */
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
        // Visible map boundary
        // ---------------------------------------------------------------------

        DrawRectangleLinesEx(
            visibleMap,
            1.0f,
            WHITE);
    }
}