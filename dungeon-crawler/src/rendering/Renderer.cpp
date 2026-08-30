#include "rendering/Renderer.h"
#include "app/Game.h"

#include "raylib.h"

namespace dungeon
{
/*
    void Renderer::draw(const Game& game) const
    {
        ClearBackground(RAYWHITE);

        const Map& map = game.map();

        for (std::size_t y = 0; y < map.height(); ++y)
        {
            for (std::size_t x = 0; x < map.width(); ++x)
            {
                const char tile = map.tileAt(
                    static_cast<int>(x),
                    static_cast<int>(y));

                const int screenX =
                    MapOffsetX +
                    static_cast<int>(x) * TileSize;

                const int screenY =
                    MapOffsetY +
                    static_cast<int>(y) * TileSize;

                if (tile == '#')
                {
                    DrawRectangle(
                        screenX,
                        screenY,
                        TileSize,
                        TileSize,
                        DARKGRAY);
                }
                else
                {
                    DrawRectangle(
                        screenX,
                        screenY,
                        TileSize,
                        TileSize,
                        LIGHTGRAY);
                }
            }
        }

        const Player& player = game.player();

        const float playerCenterX =
            static_cast<float>(
                MapOffsetX +
                player.x() * TileSize +
                TileSize / 2);

        const float playerCenterY =
            static_cast<float>(
                MapOffsetY +
                player.y() * TileSize +
                TileSize / 2);

        DrawCircle(
            static_cast<int>(playerCenterX),
            static_cast<int>(playerCenterY),
            static_cast<float>(TileSize / 3),
            MAROON);
    }
*/


    void Renderer::draw(const Game& game) const
    {
        ClearBackground(RAYWHITE);

        const Map& map = game.map();

        for (std::size_t y = 0; y < map.height(); ++y)
        {
            for (std::size_t x = 0; x < map.width(); ++x)
            {
                const char tile = map.tileAt(
                    static_cast<int>(x),
                    static_cast<int>(y));

                const int screenX =
                    MapOffsetX +
                    static_cast<int>(x) * TileSize;

                const int screenY =
                    MapOffsetY +
                    static_cast<int>(y) * TileSize;

                if (tile == '#')
                {
                    DrawRectangle(
                        screenX,
                        screenY,
                        TileSize,
                        TileSize,
                        DARKGRAY);
                }
                else
                {
                    DrawRectangle(
                        screenX,
                        screenY,
                        TileSize,
                        TileSize,
                        LIGHTGRAY);
                }
            }
        }

        const Player& player = game.player();

        const int playerCenterX =
            MapOffsetX +
            player.x() * TileSize +
            TileSize / 2;

        const int playerCenterY =
            MapOffsetY +
            player.y() * TileSize +
            TileSize / 2;

        DrawCircle(
            playerCenterX,
            playerCenterY,
            static_cast<float>(TileSize / 3),
            MAROON);

        const auto& enemies = game.enemies();

        for (const auto& enemy : enemies)
        {
            const int enemyCenterX =
                MapOffsetX +
                enemy->x() * TileSize +
                TileSize / 2;

            const int enemyCenterY =
                MapOffsetY +
                enemy->y() * TileSize +
                TileSize / 2;

            DrawCircle(
                enemyCenterX,
                enemyCenterY,
                static_cast<float>(TileSize / 3),
                RED);

            const std::string enemyType(enemy->type());

            DrawText(
                enemyType.c_str(),
                enemyCenterX - MeasureText(
                    enemyType.c_str(),
                    12) / 2,
                enemyCenterY - 6,
                12,
                BLACK);
        }
    }


}