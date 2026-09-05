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

    void Renderer::toggleGameInstructions() noexcept
    {
        m_gameInstructionsOpen = !m_gameInstructionsOpen;
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

        if (game.inCombat())
        {
            drawCombatScreenLayout();
        }
        else
        {
            drawMainScreenLayout();
        }

        // ---------------------------------------------------------------------
        // Shared Player HUD
        //
        // The Player Stats and Player Gear remain visible on both screens.
        // ---------------------------------------------------------------------

        drawStatSlots();
        drawGearSlots();

        drawTitleAssets();
        drawStatAssets(game);
        drawGearAssets(game);

        // ---------------------------------------------------------------------
        // Screen-specific rendering
        // ---------------------------------------------------------------------

        if (game.inCombat())
        {
            drawEnemyStatSlots();
            drawCombatActionBar();
        }
        else
        {
            drawMap(game);
            drawActionBar();
        }
    }

    void Renderer::drawFallbackText(
        std::string_view text,
        int x,
        int y,
        int width,
        int height,
        int fontSize) const
    {
        if (text.empty())
        {
            return;
        }

        const std::string value(text);

        const int textWidth =
            MeasureText(
                value.c_str(),
                fontSize);

        const int raylibY =
            toRaylibY(
                y,
                height);

        const int textX =
            x + (width - textWidth) / 2;

        const int textY =
            raylibY + (height - fontSize) / 2;

        DrawText(
            value.c_str(),
            textX,
            textY,
            fontSize,
            BLACK);
    }



    void Renderer::drawMainScreenLayout() const
    {
        // ---------------------------------------------------------------------
        // Title Art
        //
        // Temporary placeholder.
        // This will later be replaced by the Title Art PNG.
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.titleArt().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.titleArt(),
                TitleArtX,
                TitleArtY,
                TitleArtWidth,
                TitleArtHeight);
        }
        else
        {
            drawFallbackText(
                R"(Final C++ Project - Made by Ilan "Ilwolf" Boguslavsky/Mintzker)",
                TitleArtX,
                TitleArtY,
                TitleArtWidth,
                TitleArtHeight,
                22);
        }

        // ---------------------------------------------------------------------
        // Stats Title
        //
        // Temporary placeholder.
        // This will later be replaced by the Stats Title PNG.
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.statsTitle().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.statsTitle(),
                StatsTitleX,
                StatsTitleY,
                StatsTitleWidth,
                StatsTitleHeight);
        }
        else
        {
            drawFallbackText(
                "Player Stats",
                StatsTitleX,
                StatsTitleY,
                StatsTitleWidth,
                StatsTitleHeight,
                22);
        }

        // ---------------------------------------------------------------------
        // POV / GAME INSTRUCTIONS
        //
        // Temporary placeholder.
        // This will later be replaced by the POV PNG.
        // ---------------------------------------------------------------------

        const Rectangle povRender = toRaylibRectangle(
            PovRenderX,
            PovRenderY,
            PovRenderWidth,
            PovRenderHeight);

        if (m_gameInstructionsOpen)
        {
            DrawRectangleRec(
                povRender,
                LIGHTGRAY);

            DrawRectangleLinesEx(
                povRender,
                1.0f,
                DARKGRAY);

            drawFallbackText(
                "GAME INSTRUCTIONS",
                PovRenderX,
                PovRenderY + PovRenderHeight - 50,
                PovRenderWidth,
                40,
                26);
        }
        else
        {
            DrawRectangleRec(
                povRender,
                RAYWHITE);

            DrawRectangleLinesEx(
                povRender,
                1.0f,
                DARKGRAY);
        }

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

        if (m_mainScreenAssets.gearTitle().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.gearTitle(),
                GearTitleX,
                GearTitleY,
                GearTitleWidth,
                GearTitleHeight);
        }
        else
        {
            drawFallbackText(
                "Player Gear",
                GearTitleX,
                GearTitleY,
                GearTitleWidth,
                GearTitleHeight,
                22);
        }

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

    void Renderer::drawCombatScreenLayout() const
    {
        // ---------------------------------------------------------------------
        // Title Art
        // ---------------------------------------------------------------------

        const Rectangle titleArt =
            toRaylibRectangle(
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
        // Player Stats Title
        // ---------------------------------------------------------------------

        const Rectangle statsTitle =
            toRaylibRectangle(
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
        // Player Stats Window
        // ---------------------------------------------------------------------

        const Rectangle statsWindow =
            toRaylibRectangle(
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

        const Rectangle gearTitle =
            toRaylibRectangle(
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

        const Rectangle gearWindow =
            toRaylibRectangle(
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
        // Enemy Stats Title
        // ---------------------------------------------------------------------

        const Rectangle enemyStatsTitle =
            toRaylibRectangle(
                0,
                620,
                230,
                50);

        DrawRectangleRec(
            enemyStatsTitle,
            LIGHTGRAY);

        DrawRectangleLinesEx(
            enemyStatsTitle,
            1.0f,
            DARKGRAY);

        drawFallbackText(
            "Enemy Stats",
            0,
            620,
            230,
            50,
            20);

        // ---------------------------------------------------------------------
        // Enemy Stats Window
        // ---------------------------------------------------------------------

        const Rectangle enemyStatsWindow =
            toRaylibRectangle(
                0,
                260,
                230,
                360);

        DrawRectangleRec(
            enemyStatsWindow,
            RAYWHITE);

        DrawRectangleLinesEx(
            enemyStatsWindow,
            1.0f,
            DARKGRAY);

        // ---------------------------------------------------------------------
        // Dice Roll Title
        // ---------------------------------------------------------------------

        const Rectangle diceRollTitle =
            toRaylibRectangle(
                230,
                620,
                240,
                50);

        DrawRectangleRec(
            diceRollTitle,
            LIGHTGRAY);

        DrawRectangleLinesEx(
            diceRollTitle,
            1.0f,
            DARKGRAY);

        drawFallbackText(
            "Dice Roll",
            230,
            620,
            240,
            50,
            20);

        // ---------------------------------------------------------------------
        // Dice Roll Window
        // ---------------------------------------------------------------------

        const Rectangle diceRollWindow =
            toRaylibRectangle(
                230,
                260,
                240,
                360);

        DrawRectangleRec(
            diceRollWindow,
            RAYWHITE);

        DrawRectangleLinesEx(
            diceRollWindow,
            1.0f,
            DARKGRAY);

        drawFallbackText(
            "Dice Roll Window",
            230,
            260 + 145,
            240,
            70,
            18);

        // ---------------------------------------------------------------------
        // Combat Info Title
        // ---------------------------------------------------------------------

        const Rectangle combatInfoTitle =
            toRaylibRectangle(
                230,
                360,
                240,
                50);

        DrawRectangleRec(
            combatInfoTitle,
            LIGHTGRAY);

        DrawRectangleLinesEx(
            combatInfoTitle,
            1.0f,
            DARKGRAY);

        drawFallbackText(
            "Combat Info",
            230,
            360,
            240,
            50,
            18);

        // ---------------------------------------------------------------------
        // Combat Info Window
        // ---------------------------------------------------------------------

        const Rectangle combatInfoWindow =
            toRaylibRectangle(
                230,
                260,
                240,
                100);

        DrawRectangleRec(
            combatInfoWindow,
            RAYWHITE);

        DrawRectangleLinesEx(
            combatInfoWindow,
            1.0f,
            DARKGRAY);

        drawFallbackText(
            "Combat Info Window",
            230,
            260,
            240,
            100,
            16);

        // ---------------------------------------------------------------------
        // Loot Title
        // ---------------------------------------------------------------------

        const Rectangle lootTitle =
            toRaylibRectangle(
                0,
                200,
                104,
                60);

        DrawRectangleRec(
            lootTitle,
            LIGHTGRAY);

        DrawRectangleLinesEx(
            lootTitle,
            1.0f,
            DARKGRAY);

        drawFallbackText(
            "Loot",
            0,
            200,
            104,
            60,
            18);

        // ---------------------------------------------------------------------
        // Loot Table Window
        // ---------------------------------------------------------------------

        const int lootTableX = 104;
        const int lootTableY = 200;
        const int lootTableWidth = 336;
        const int lootTableHeight = 60;

        const Rectangle lootTableWindow =
            toRaylibRectangle(
                lootTableX,
                lootTableY,
                lootTableWidth,
                lootTableHeight);

        DrawRectangleRec(
            lootTableWindow,
            RAYWHITE);

        // Left edge.
        DrawLineEx(
            Vector2{
                static_cast<float>(lootTableX),
                static_cast<float>(
                    toRaylibY(
                        lootTableY,
                        lootTableHeight))
            },
            Vector2{
                static_cast<float>(lootTableX),
                static_cast<float>(
                    toRaylibY(
                        lootTableY,
                        lootTableHeight) +
                    lootTableHeight)
            },
            1.0f,
            DARKGRAY);

        // Top and bottom edges.
        DrawLineEx(
            Vector2{
                static_cast<float>(lootTableX),
                static_cast<float>(
                    toRaylibY(
                        lootTableY,
                        lootTableHeight))
            },
            Vector2{
                static_cast<float>(
                    lootTableX +
                    lootTableWidth),
                static_cast<float>(
                    toRaylibY(
                        lootTableY,
                        lootTableHeight))
            },
            1.0f,
            DARKGRAY);

        DrawLineEx(
            Vector2{
                static_cast<float>(lootTableX),
                static_cast<float>(
                    toRaylibY(
                        lootTableY,
                        lootTableHeight) +
                    lootTableHeight)
            },
            Vector2{
                static_cast<float>(
                    lootTableX +
                    lootTableWidth),
                static_cast<float>(
                    toRaylibY(
                        lootTableY,
                        lootTableHeight) +
                    lootTableHeight)
            },
            1.0f,
            DARKGRAY);

        // ---------------------------------------------------------------------
        // Loot Icon Slots
        // ---------------------------------------------------------------------

        constexpr int LootIconSize = 48;
        constexpr int LootIconY = 206;

        constexpr int LootIcon1X = 125;
        constexpr int LootIcon2X = 194;
        constexpr int LootIcon3X = 263;
        constexpr int LootIcon4X = 332;
        constexpr int LootIcon5X = 401;

        const int lootIconXs[] =
        {
            LootIcon1X,
            LootIcon2X,
            LootIcon3X,
            LootIcon4X,
            LootIcon5X
        };

        for (const int x : lootIconXs)
        {
            DrawRectangleLinesEx(
                toRaylibRectangle(
                    x,
                    LootIconY,
                    LootIconSize,
                    LootIconSize),
                1.0f,
                GRAY);
        }

        // ---------------------------------------------------------------------
        // POV / Combat Instructions
        // ---------------------------------------------------------------------

        const Rectangle povRender =
            toRaylibRectangle(
                PovRenderX,
                PovRenderY,
                PovRenderWidth,
                PovRenderHeight);

        if (m_gameInstructionsOpen)
        {
            DrawRectangleRec(
                povRender,
                LIGHTGRAY);

            DrawRectangleLinesEx(
                povRender,
                1.0f,
                DARKGRAY);

            drawFallbackText(
                "GAME COMBAT INSTRUCTIONS",
                PovRenderX,
                PovRenderY + 330,
                PovRenderWidth,
                50,
                24);

            drawFallbackText(
                "Combat rules will be displayed here.",
                PovRenderX,
                PovRenderY + 265,
                PovRenderWidth,
                40,
                18);

            drawFallbackText(
                "Press TAB to return to the Combat POV.",
                PovRenderX,
                PovRenderY + 35,
                PovRenderWidth,
                35,
                16);
        }
        else
        {
            DrawRectangleRec(
                povRender,
                RAYWHITE);

            DrawRectangleLinesEx(
                povRender,
                1.0f,
                DARKGRAY);

            drawFallbackText(
                "POV ART",
                PovRenderX,
                PovRenderY + 210,
                PovRenderWidth,
                50,
                28);

            drawFallbackText(
                "580x470",
                PovRenderX,
                PovRenderY + 145,
                PovRenderWidth,
                50,
                24);
        }

    }

    void Renderer::drawEnemyStatSlots() const
    {
        const int iconX = EnemyStatIconX;
        const int numberX = EnemyStatNumberX;

        struct EnemyStatSlot
        {
            const char* label;
            int y;
        };

        constexpr EnemyStatSlot slots[] =
        {
            { "ATK",  EnemyStatAtkY },
            { "STR",  EnemyStatStrY },
            { "PRE",  EnemyStatPrecY },
            { "DMG",  EnemyStatDmgY },
            { "DEF",  EnemyStatDefY },
            { "TGH",  EnemyStatToughY },
            { "HP",   EnemyStatHpY }
        };

        for (const EnemyStatSlot& slot : slots)
        {
            DrawRectangleLinesEx(
                toRaylibRectangle(
                    iconX,
                    slot.y,
                    StatIconSize,
                    StatIconSize),
                1.0f,
                GRAY);

            DrawRectangleLinesEx(
                toRaylibRectangle(
                    numberX,
                    slot.y,
                    StatNumberSize,
                    StatNumberSize),
                1.0f,
                GRAY);

            drawFallbackText(
                slot.label,
                iconX,
                slot.y,
                StatIconSize,
                StatIconSize,
                10);

            drawFallbackText(
                "--",
                numberX,
                slot.y,
                StatNumberSize,
                StatNumberSize,
                12);
        }
    }

    void Renderer::drawStatSlots() const
    {
        // ---------------------------------------------------------------------
        // Temporary Stat PNG placeholders
        //
        // Every slot is exactly 32x32.
        //
        // The final Stat Icon / Number PNGs will be drawn in these exact
        // positions.
        // so These temporary rectangles can then be commented out.
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


        //ATK Icon
        if (m_mainScreenAssets.atkIcon().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.atkIcon(),
                StatIconX,
                StatAtkY,
                StatIconSize,
                StatIconSize);
        }
        else
        {
            drawFallbackText(
                "ATK",
                StatIconX,
                StatAtkY,
                StatIconSize,
                StatIconSize,
                10);
        }


        //STR Icon
        if (m_mainScreenAssets.strIcon().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.strIcon(),
                StatIconX,
                StatStrY,
                StatIconSize,
                StatIconSize);
        }
        else
        {
            drawFallbackText(
                "STR",
                StatIconX,
                StatStrY,
                StatIconSize,
                StatIconSize,
                10);
        }


        //PREC Icon
        if (m_mainScreenAssets.precIcon().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.precIcon(),
                StatIconX,
                StatPrecY,
                StatIconSize,
                StatIconSize);
        }
        else
        {
            drawFallbackText(
                "PREC",
                StatIconX,
                StatPrecY,
                StatIconSize,
                StatIconSize,
                10);
        }


        //DMG Icon
        if (m_mainScreenAssets.dmgIcon().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.dmgIcon(),
                StatIconX,
                StatDmgY,
                StatIconSize,
                StatIconSize);
        }
        else
        {
            drawFallbackText(
                "DMG",
                StatIconX,
                StatDmgY,
                StatIconSize,
                StatIconSize,
                10);
        }


        //DEF Icon
        if (m_mainScreenAssets.defIcon().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.defIcon(),
                StatIconX,
                StatDefY,
                StatIconSize,
                StatIconSize);
        }
        else
        {
            drawFallbackText(
                "DEF",
                StatIconX,
                StatDefY,
                StatIconSize,
                StatIconSize,
                10);
        }


        //TOUGH Icon
        if (m_mainScreenAssets.toughIcon().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.toughIcon(),
                StatIconX,
                StatToughY,
                StatIconSize,
                StatIconSize);
        }
        else
        {
            drawFallbackText(
                "TGH",
                StatIconX,
                StatToughY,
                StatIconSize,
                StatIconSize,
                10);
        }


        //HP Icon
        if (m_mainScreenAssets.hpIcon().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.hpIcon(),
                StatIconX,
                StatHpY,
                StatIconSize,
                StatIconSize);
        }
        else
        {
            drawFallbackText(
                "HP",
                StatIconX,
                StatHpY,
                StatIconSize,
                StatIconSize,
                10);
        }



        // -------------------------------------------------------------------------
        // Dynamic Number PNGs
        // -------------------------------------------------------------------------


        
        //ATK Num
        const Texture2D& attackNumber =
            m_mainScreenAssets.number(attack);

        if (attackNumber.id != 0)
        {
            drawTexture(
                attackNumber,
                StatNumberX,
                StatAtkY,
                StatNumberSize,
                StatNumberSize);
        }
        else
        {
            drawFallbackText(
                std::to_string(attack),
                StatNumberX,
                StatAtkY,
                StatNumberSize,
                StatNumberSize,
                18);
        }



        //STR Num
        const Texture2D& strengthNumber =
            m_mainScreenAssets.number(strength);

        if (strengthNumber.id != 0)
        {
            drawTexture(
                strengthNumber,
                StatNumberX,
                StatStrY,
                StatNumberSize,
                StatNumberSize);
        }
        else
        {
            drawFallbackText(
                std::to_string(strength),
                StatNumberX,
                StatStrY,
                StatNumberSize,
                StatNumberSize,
                18);
        }



        //PREC Num
        const Texture2D& precisionNumber =
            m_mainScreenAssets.number(precision);

        if (precisionNumber.id != 0)
        {
            drawTexture(
                precisionNumber,
                StatNumberX,
                StatPrecY,
                StatNumberSize,
                StatNumberSize);
        }
        else
        {
            drawFallbackText(
                std::to_string(precision),
                StatNumberX,
                StatPrecY,
                StatNumberSize,
                StatNumberSize,
                18);
        }



        //DMG Num
        const Texture2D& damageNumber =
            m_mainScreenAssets.number(damage);

        if (damageNumber.id != 0)
        {
            drawTexture(
                damageNumber,
                StatNumberX,
                StatDmgY,
                StatNumberSize,
                StatNumberSize);
        }
        else
        {
            drawFallbackText(
                std::to_string(damage),
                StatNumberX,
                StatDmgY,
                StatNumberSize,
                StatNumberSize,
                18);
        }



        //DEF Num
        const Texture2D& defenseNumber =
            m_mainScreenAssets.number(defense);

        if (defenseNumber.id != 0)
        {
            drawTexture(
                defenseNumber,
                StatNumberX,
                StatDefY,
                StatNumberSize,
                StatNumberSize);
        }
        else
        {
            drawFallbackText(
                std::to_string(defense),
                StatNumberX,
                StatDefY,
                StatNumberSize,
                StatNumberSize,
                18);
        }



        //TOUGH Num
        const Texture2D& toughnessNumber =
            m_mainScreenAssets.number(toughness);

        if (toughnessNumber.id != 0)
        {
            drawTexture(
                toughnessNumber,
                StatNumberX,
                StatToughY,
                StatNumberSize,
                StatNumberSize);
        }
        else
        {
            drawFallbackText(
                std::to_string(toughness),
                StatNumberX,
                StatToughY,
                StatNumberSize,
                StatNumberSize,
                18);
        }



        //HP Num
        const Texture2D& hpNumber =
            m_mainScreenAssets.number(hp);

        if (hpNumber.id != 0)
        {
            drawTexture(
                hpNumber,
                StatNumberX,
                StatHpY,
                StatNumberSize,
                StatNumberSize);
        }
        else
        {
            drawFallbackText(
                std::to_string(hp),
                StatNumberX,
                StatHpY,
                StatNumberSize,
                StatNumberSize,
                18);
        }
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

        // Health Potion Icon
        if (m_mainScreenAssets.healthPotionIcon().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.healthPotionIcon(),
                PotionIconX,
                HealthPotionY,
                GearIconSize,
                GearIconSize);
        }
        else
        {
            drawFallbackText(
                "HPC",
                PotionIconX,
                HealthPotionY,
                GearIconSize,
                GearIconSize,
                14);
        }


        // Rage Potion Icon
        if (m_mainScreenAssets.ragePotionIcon().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.ragePotionIcon(),
                PotionIconX,
                RagePotionY,
                GearIconSize,
                GearIconSize);
        }
        else
        {
            drawFallbackText(
                "RPC",
                PotionIconX,
                RagePotionY,
                GearIconSize,
                GearIconSize,
                14);
        }

        // -------------------------------------------------------------------------
        // Potion Amounts
        // -------------------------------------------------------------------------


        // Health Potion Amount
        const int healthAmount =
            static_cast<int>(
                inventory.amount(HealthPotion{}));

        const Texture2D& healthAmountTexture =
            m_mainScreenAssets.healthPotionAmount(
                healthAmount);

        if (healthAmountTexture.id != 0)
        {
            drawTexture(
                healthAmountTexture,
                PotionNumberX,
                HealthPotionY,
                GearIconSize,
                GearIconSize);
        }
        else
        {
            drawFallbackText(
                std::to_string(healthAmount) + "/3",
                PotionNumberX,
                HealthPotionY,
                GearIconSize,
                GearIconSize,
                18);
        }


        // Rage Potion Amount
        const int rageAmount =
            static_cast<int>(
                inventory.amount(RagePotion{}));

        const Texture2D& rageAmountTexture =
            m_mainScreenAssets.ragePotionAmount(
                rageAmount);

        if (rageAmountTexture.id != 0)
        {
            drawTexture(
                rageAmountTexture,
                PotionNumberX,
                RagePotionY,
                GearIconSize,
                GearIconSize);
        }
        else
        {
            drawFallbackText(
                std::to_string(rageAmount) + "/3",
                PotionNumberX,
                RagePotionY,
                GearIconSize,
                GearIconSize,
                18);
        }

        // -------------------------------------------------------------------------
        // Weapon
        // -------------------------------------------------------------------------

        const Weapon* weapon =
            player.equipment().weapon();

        if (weapon != nullptr)
        {
            const Texture2D& weaponTexture =
                m_mainScreenAssets.weaponTier(
                    weapon->tier());

            if (weaponTexture.id != 0)
            {
                drawTexture(
                    weaponTexture,
                    WeaponIconX,
                    WeaponArmorY,
                    GearIconSize,
                    GearIconSize);
            }
            else
            {
                drawFallbackText(
                    std::string("W") + std::to_string(weapon->tier()),
                    WeaponIconX,
                    WeaponArmorY,
                    GearIconSize,
                    GearIconSize,
                    14);
            }
        }
        else
        {
            drawFallbackText(
                "--",
                WeaponIconX,
                WeaponArmorY,
                GearIconSize,
                GearIconSize,
                14);
        }

        // -------------------------------------------------------------------------
        // Armor
        // -------------------------------------------------------------------------

        const Armor* armor =
            player.equipment().armor();

        if (armor != nullptr)
        {
            const Texture2D& armorTexture =
                m_mainScreenAssets.armorTier(
                    armor->tier());

            if (armorTexture.id != 0)
            {
                drawTexture(
                    armorTexture,
                    ArmorIconX,
                    WeaponArmorY,
                    GearIconSize,
                    GearIconSize);
            }
            else
            {
                drawFallbackText(
                    std::string("A") + std::to_string(armor->tier()),
                    ArmorIconX,
                    WeaponArmorY,
                    GearIconSize,
                    GearIconSize,
                    14);
            }
        }
        else
        {
            drawFallbackText(
                "--",
                ArmorIconX,
                WeaponArmorY,
                GearIconSize,
                GearIconSize,
                14);
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

        // Magic Skull
        if (hasMagicSkull)
        {
            const Texture2D& texture =
                m_mainScreenAssets.magicSkull();

            if (texture.id != 0)
            {
                drawTexture(
                    texture,
                    Tier1AccessoryX,
                    AccessoriesY,
                    GearIconSize,
                    GearIconSize);
            }
            else
            {
                drawFallbackText(
                    "MS",
                    Tier1AccessoryX,
                    AccessoriesY,
                    GearIconSize,
                    GearIconSize,
                    14);
            }
        }
        else
        {
            const Texture2D& texture =
                m_mainScreenAssets.noAccessory();

            if (texture.id != 0)
            {
                drawTexture(
                    texture,
                    Tier1AccessoryX,
                    AccessoriesY,
                    GearIconSize,
                    GearIconSize);
            }
            else
            {
                drawFallbackText(
                    "--",
                    Tier1AccessoryX,
                    AccessoriesY,
                    GearIconSize,
                    GearIconSize,
                    14);
            }
        }

        // Orc Fang
        if (hasOrcFang)
        {
            const Texture2D& texture =
                m_mainScreenAssets.orcFang();

            if (texture.id != 0)
            {
                drawTexture(
                    texture,
                    Tier2AccessoryX,
                    AccessoriesY,
                    GearIconSize,
                    GearIconSize);
            }
            else
            {
                drawFallbackText(
                    "OF",
                    Tier2AccessoryX,
                    AccessoriesY,
                    GearIconSize,
                    GearIconSize,
                    14);
            }
        }
        else
        {
            const Texture2D& texture =
                m_mainScreenAssets.noAccessory();

            if (texture.id != 0)
            {
                drawTexture(
                    texture,
                    Tier2AccessoryX,
                    AccessoriesY,
                    GearIconSize,
                    GearIconSize);
            }
            else
            {
                drawFallbackText(
                    "--",
                    Tier2AccessoryX,
                    AccessoriesY,
                    GearIconSize,
                    GearIconSize,
                    14);
            }
        }

        // Troll's Heart
        if (hasTrollHeart)
        {
            const Texture2D& texture =
                m_mainScreenAssets.trollHeart();

            if (texture.id != 0)
            {
                drawTexture(
                    texture,
                    Tier3AccessoryX,
                    AccessoriesY,
                    GearIconSize,
                    GearIconSize);
            }
            else
            {
                drawFallbackText(
                    "TH",
                    Tier3AccessoryX,
                    AccessoriesY,
                    GearIconSize,
                    GearIconSize,
                    14);
            }
        }
        else
        {
            const Texture2D& texture =
                m_mainScreenAssets.noAccessory();

            if (texture.id != 0)
            {
                drawTexture(
                    texture,
                    Tier3AccessoryX,
                    AccessoriesY,
                    GearIconSize,
                    GearIconSize);
            }
            else
            {
                drawFallbackText(
                    "--",
                    Tier3AccessoryX,
                    AccessoriesY,
                    GearIconSize,
                    GearIconSize,
                    14);
            }
        }
    }



        // ---------------------------------------------------------------------
        // Action Render area
        // ---------------------------------------------------------------------

    void Renderer::drawActionBar() const
    {
        const Rectangle actionBar =
            toRaylibRectangle(
                ActionBarX,
                ActionBarY,
                ActionBarWidth,
                ActionBarHeight);

        if (m_mainScreenAssets.traversalActionBar().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.traversalActionBar(),
                ActionBarX,
                ActionBarY,
                ActionBarWidth,
                ActionBarHeight);

            return;
        }

        // ---------------------------------------------------------------------
        // Temporary traversal action-bar fallback.
        // ---------------------------------------------------------------------

        DrawRectangleRec(
            actionBar,
            LIGHTGRAY);

        DrawRectangleLinesEx(
            actionBar,
            1.0f,
            DARKGRAY);

        drawFallbackText(
            "TRAVERSAL CONTROLS",
            ActionBarX,
            ActionBarY + 140,
            ActionBarWidth,
            40,
            22);

        drawFallbackText(
            "MOVE",
            ActionBarX + 40,
            ActionBarY + 85,
            220,
            35,
            18);

        drawFallbackText(
            "ATTACK",
            ActionBarX + 270,
            ActionBarY + 85,
            220,
            35,
            18);

        drawFallbackText(
            "INVENTORY",
            ActionBarX + 500,
            ActionBarY + 85,
            220,
            35,
            18);

        drawFallbackText(
            "ESCAPE",
            ActionBarX + 730,
            ActionBarY + 85,
            220,
            35,
            18);

        drawFallbackText(
            "TAB  -  Game Instructions",
            ActionBarX,
            ActionBarY + 20,
            ActionBarWidth,
            35,
            16);
    }


    void Renderer::drawCombatActionBar() const
    {
        const Rectangle actionBar =
            toRaylibRectangle(
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

        // ---------------------------------------------------------------------
        // Heading
        // ---------------------------------------------------------------------

        drawFallbackText(
            "COMBAT ACTIONS",
            ActionBarX,
            ActionBarY + 145,
            ActionBarWidth,
            35,
            22);

        // ---------------------------------------------------------------------
        // Actions
        // ---------------------------------------------------------------------

        drawFallbackText(
            "F1 / 1  -  ATTACK",
            ActionBarX + 25,
            ActionBarY + 85,
            250,
            35,
            17);

        drawFallbackText(
            "I / 2  -  INVENTORY",
            ActionBarX + 275,
            ActionBarY + 85,
            250,
            35,
            17);

        drawFallbackText(
            "E / 3  -  ESCAPE",
            ActionBarX + 525,
            ActionBarY + 85,
            250,
            35,
            17);

        drawFallbackText(
            "TAB  -  COMBAT INSTRUCTIONS",
            ActionBarX,
            ActionBarY + 25,
            ActionBarWidth,
            35,
            17);
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
                const char* identifier = "G";

                if (type == "Skeleton")
                {
                    identifier = "S";
                }
                else if (type == "Orc")
                {
                    identifier = "O";
                }
                else if (type == "Troll")
                {
                    identifier = "T";
                }
                else if (type == "Dragon")
                {
                    identifier = "D";
                }

                DrawRectangle(
                    enemyX,
                    toRaylibY(
                        enemyY,
                        TemporaryMapTileSize),
                    TemporaryMapTileSize,
                    TemporaryMapTileSize,
                    RED);

                drawFallbackText(
                    identifier,
                    enemyX,
                    enemyY,
                    TemporaryMapTileSize,
                    TemporaryMapTileSize,
                    11);
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