#include "rendering/Renderer.h"
#include "app/Game.h"

#include "raylib.h"


#include "combat/Combat.h"
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
#include <vector>

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

    // =========================================================================
    // Coordinate Conversion
    // =========================================================================

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
            static_cast<float>(
                toRaylibY(
                    y,
                    height)),
            static_cast<float>(width),
            static_cast<float>(height)
        };
    }

    // =========================================================================
    // Asset Loading
    // =========================================================================

    bool Renderer::loadMainScreenAssets()
    {
        return m_mainScreenAssets.load();
    }

    // =========================================================================
    // Instructions Toggle
    // =========================================================================

    void Renderer::toggleGameInstructions() noexcept
    {
        m_gameInstructionsOpen =
            !m_gameInstructionsOpen;
    }

    // =========================================================================
    // Basic Texture Rendering
    // =========================================================================

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

    // =========================================================================
    // Fallback Text
    // =========================================================================

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
            x +
            (width - textWidth) / 2;

        const int textY =
            raylibY +
            (height - fontSize) / 2;

        DrawText(
            value.c_str(),
            textX,
            textY,
            fontSize,
            BLACK);
    }

    // =========================================================================
    // Wrapped Fallback Text
    //
    // Supports:
    //   - explicit '\n' line breaks
    //   - automatic word wrapping
    //   - horizontal centering
    //   - vertical centering of the COMPLETE text block
    //
    // Designed for small UI panels such as Combat Info.
    // =========================================================================

    void Renderer::drawWrappedFallbackText(
        std::string_view text,
        int x,
        int y,
        int width,
        int height,
        int fontSize,
        int lineSpacing) const
    {
        if (text.empty())
        {
            return;
        }

        const std::string value(text);

        constexpr int HorizontalPadding = 12;

        const int maxTextWidth =
            width - (HorizontalPadding * 2);

        if (maxTextWidth <= 0 || height <= 0)
        {
            return;
        }

        // -------------------------------------------------------------------------
        // Build wrapped lines while preserving explicit '\n' line breaks.
        // -------------------------------------------------------------------------

        std::vector<std::string> lines;
        std::string currentLine;

        auto pushCurrentLine =
            [&]()
            {
                if (!currentLine.empty())
                {
                    lines.push_back(currentLine);
                    currentLine.clear();
                }
            };

        std::size_t start = 0;

        while (start <= value.size())
        {
            const std::size_t newline =
                value.find('\n', start);

            const std::size_t sectionEnd =
                newline == std::string::npos
                ? value.size()
                : newline;

            std::size_t wordStart = start;

            while (wordStart < sectionEnd)
            {
                std::size_t wordEnd =
                    value.find(' ', wordStart);

                if (wordEnd == std::string::npos ||
                    wordEnd > sectionEnd)
                {
                    wordEnd = sectionEnd;
                }

                const std::string word =
                    value.substr(
                        wordStart,
                        wordEnd - wordStart);

                if (!word.empty())
                {
                    const std::string candidate =
                        currentLine.empty()
                        ? word
                        : currentLine + " " + word;

                    if (MeasureText(
                        candidate.c_str(),
                        fontSize) <= maxTextWidth)
                    {
                        currentLine = candidate;
                    }
                    else
                    {
                        pushCurrentLine();

                        currentLine = word;
                    }
                }

                if (wordEnd >= sectionEnd)
                {
                    break;
                }

                wordStart = wordEnd + 1;
            }

            // Explicit newline.
            pushCurrentLine();

            if (newline == std::string::npos)
            {
                break;
            }

            start = newline + 1;
        }

        if (lines.empty())
        {
            return;
        }

        // -------------------------------------------------------------------------
        // Make sure line spacing is always large enough for the font.
        //
        // We want the lines to be visually separated, not overlapping.
        // -------------------------------------------------------------------------

        const int actualLineSpacing =
            (lineSpacing < fontSize + 2)
            ? fontSize + 2
            : lineSpacing;

        // -------------------------------------------------------------------------
        // Calculate the COMPLETE text block height.
        // -------------------------------------------------------------------------

        const int totalHeight =
            fontSize +
            static_cast<int>(lines.size() - 1) *
            actualLineSpacing;

        // -------------------------------------------------------------------------
        // Center the COMPLETE block inside the supplied window.
        //
        // IMPORTANT:
        // My design coordinates use a bottom-left origin.
        //
        // Therefore:
        //   - the FIRST line is the TOP line
        //   - subsequent lines move DOWN by decreasing Y
        //
        // sooo I first calculate the bottom of the entire block, then move upward
        // to find the Y position of the first/top line.
        // 
        // DO NOT FORGET IT!
        // -------------------------------------------------------------------------


        const int blockBottomY =
            y +
            (height - totalHeight) / 2;

        const int firstLineY =
            blockBottomY +
            static_cast<int>(lines.size() - 1) *
            lineSpacing;

        int currentY =
            firstLineY;

        // -------------------------------------------------------------------------
        // Draw each line centered horizontally.
        // -------------------------------------------------------------------------

        //int currentY = firstLineY;

        for (const std::string& line : lines)
        {
            const int lineWidth =
                MeasureText(
                    line.c_str(),
                    fontSize);

            const int lineX =
                x +
                (width - lineWidth) / 2;

            drawFallbackText(
                line,
                lineX,
                currentY,
                lineWidth,
                fontSize,
                fontSize);

            currentY -= actualLineSpacing;
        }
    }

    // =========================================================================
    // Main Renderer
    // =========================================================================

    void Renderer::draw(
        const Game& game) const
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
        // ---------------------------------------------------------------------

        drawStatSlots();
        drawGearSlots();

        drawTitleAssets();
        drawStatAssets(game);
        drawGearAssets(game);

        // ---------------------------------------------------------------------
        // Screen-specific content
        // ---------------------------------------------------------------------

        if (game.inCombat())
        {
            drawEnemyStatSlots(game);
            drawDiceRoll(game);
            drawCombatInfo(game);
            drawCombatActionBar();
        }
        else
        {
            drawMap(game);
            drawActionBar();
        }
    }

    // =========================================================================
    // Main Screen Layout
    // =========================================================================

    void Renderer::drawMainScreenLayout() const
    {
        // ---------------------------------------------------------------------
        // Title Art
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.titleArt().id == 0)
        {
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
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.statsTitle().id == 0)
        {
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

            drawFallbackText(
                "Player Stats",
                StatsTitleX,
                StatsTitleY,
                StatsTitleWidth,
                StatsTitleHeight,
                22);
        }

        // ---------------------------------------------------------------------
        // POV / Game Instructions
        // ---------------------------------------------------------------------

        const Rectangle povRender =
            toRaylibRectangle(
                PovRenderX,
                PovRenderY,
                PovRenderWidth,
                PovRenderHeight);

        DrawRectangleRec(
            povRender,
            m_gameInstructionsOpen
            ? LIGHTGRAY
            : RAYWHITE);

        DrawRectangleLinesEx(
            povRender,
            1.0f,
            DARKGRAY);

        if (m_gameInstructionsOpen)
        {
            drawFallbackText(
                "GAME INSTRUCTIONS",
                PovRenderX,
                PovRenderY + PovRenderHeight - 50,
                PovRenderWidth,
                40,
                26);
        }

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

        if (m_mainScreenAssets.gearTitle().id == 0)
        {
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
        // Traversal Action Bar background
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.traversalActionBar().id == 0)
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
        }
    }

    // =========================================================================
    // Combat Screen Layout
    // =========================================================================

    void Renderer::drawCombatScreenLayout() const
    {
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
            GRAY);

        DrawRectangleLinesEx(
            diceRollWindow,
            1.0f,
            DARKGRAY);

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
        //
        // Nominal window remains exactly 336x60, as specified.
        //
        // Its right border is intentionally omitted because Loot Icon 5
        // extends beyond X = 440 and ends at X = 449.
        // ---------------------------------------------------------------------

        constexpr int LootTableX = 104;
        constexpr int LootTableY = 200;
        constexpr int LootTableWidth = 336;
        constexpr int LootTableHeight = 60;

        const Rectangle lootTableWindow =
            toRaylibRectangle(
                LootTableX,
                LootTableY,
                LootTableWidth,
                LootTableHeight);

        DrawRectangleRec(
            lootTableWindow,
            RAYWHITE);

        // Left edge.
        DrawLineEx(
            Vector2{
                static_cast<float>(LootTableX),
                static_cast<float>(
                    toRaylibY(
                        LootTableY,
                        LootTableHeight))
            },
            Vector2{
                static_cast<float>(LootTableX),
                static_cast<float>(
                    toRaylibY(
                        LootTableY,
                        LootTableHeight) +
                    LootTableHeight)
            },
            1.0f,
            DARKGRAY);

        // Top edge.
        DrawLineEx(
            Vector2{
                static_cast<float>(LootTableX),
                static_cast<float>(
                    toRaylibY(
                        LootTableY,
                        LootTableHeight))
            },
            Vector2{
                static_cast<float>(
                    LootTableX +
                    LootTableWidth),
                static_cast<float>(
                    toRaylibY(
                        LootTableY,
                        LootTableHeight))
            },
            1.0f,
            DARKGRAY);

        // Bottom edge.
        DrawLineEx(
            Vector2{
                static_cast<float>(LootTableX),
                static_cast<float>(
                    toRaylibY(
                        LootTableY,
                        LootTableHeight) +
                    LootTableHeight)
            },
            Vector2{
                static_cast<float>(
                    LootTableX +
                    LootTableWidth),
                static_cast<float>(
                    toRaylibY(
                        LootTableY,
                        LootTableHeight) +
                    LootTableHeight)
            },
            1.0f,
            DARKGRAY);

        // ---------------------------------------------------------------------
        // Loot slots
        // ---------------------------------------------------------------------

        constexpr int LootIconSize = 48;
        constexpr int LootIconY = 206;

        constexpr int LootIcon1X = 125;
        constexpr int LootIcon2X = 194;
        constexpr int LootIcon3X = 263;
        constexpr int LootIcon4X = 332;
        constexpr int LootIcon5X = 401;

        constexpr int LootIconXs[] =
        {
            LootIcon1X,
            LootIcon2X,
            LootIcon3X,
            LootIcon4X,
            LootIcon5X
        };

        for (const int iconX : LootIconXs)
        {
            DrawRectangleLinesEx(
                toRaylibRectangle(
                    iconX,
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

        DrawRectangleRec(
            povRender,
            m_gameInstructionsOpen
            ? LIGHTGRAY
            : RAYWHITE);

        DrawRectangleLinesEx(
            povRender,
            1.0f,
            DARKGRAY);

        if (m_gameInstructionsOpen)
        {
            drawFallbackText(
                "GAME COMBAT INSTRUCTIONS",
                PovRenderX,
                PovRenderY + 330,
                PovRenderWidth,
                50,
                24);

            drawWrappedFallbackText(
                "Combat rules will be displayed here.",
                PovRenderX + 20,
                PovRenderY + 195,
                PovRenderWidth - 40,
                80,
                18,
                24);

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

        // ---------------------------------------------------------------------
        // Combat Action Bar background
        //
        // Actual fallback contents are drawn by drawCombatActionBar().
        // ---------------------------------------------------------------------
    }

    // =========================================================================
    // Titles
    // =========================================================================

    void Renderer::drawTitleAssets() const
    {
        if (m_mainScreenAssets.titleArt().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.titleArt(),
                TitleArtX,
                TitleArtY,
                TitleArtWidth,
                TitleArtHeight);
        }

        if (m_mainScreenAssets.statsTitle().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.statsTitle(),
                StatsTitleX,
                StatsTitleY,
                StatsTitleWidth,
                StatsTitleHeight);
        }

        if (m_mainScreenAssets.gearTitle().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.gearTitle(),
                GearTitleX,
                GearTitleY,
                GearTitleWidth,
                GearTitleHeight);
        }
    }

    // =========================================================================
    // Player Stats Slot Fallback
    // =========================================================================

    void Renderer::drawStatSlots() const
    {
        const int iconX =
            StatIconX;

        const int numberX =
            StatNumberX;

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
            DrawRectangleLinesEx(
                toRaylibRectangle(
                    iconX,
                    y,
                    StatIconSize,
                    StatIconSize),
                1.0f,
                GRAY);

            DrawRectangleLinesEx(
                toRaylibRectangle(
                    numberX,
                    y,
                    StatNumberSize,
                    StatNumberSize),
                1.0f,
                GRAY);
        }
    }

    // =========================================================================
    // Enemy Stats Slot Fallback
    // =========================================================================

    void Renderer::drawEnemyStatSlots(
        const Game& game) const
    {
        const int iconX =
            EnemyStatIconX;

        const int numberX =
            EnemyStatNumberX;

        struct EnemyStatSlot
        {
            const char* label;
            int y;
            int value;
        };

        // -------------------------------------------------------------------------
        // No active combat target.
        // -------------------------------------------------------------------------

        const Combat* combat =
            game.combat();

        if (combat == nullptr)
        {
            constexpr EnemyStatSlot emptySlots[] =
            {
                { "ATK", EnemyStatAtkY, 0 },
                { "STR", EnemyStatStrY, 0 },
                { "PRE", EnemyStatPrecY, 0 },
                { "DMG", EnemyStatDmgY, 0 },
                { "DEF", EnemyStatDefY, 0 },
                { "TGH", EnemyStatToughY, 0 },
                { "HP",  EnemyStatHpY, 0 }
            };

            for (const EnemyStatSlot& slot : emptySlots)
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

            return;
        }

        // -------------------------------------------------------------------------
        // Active combat target.
        // -------------------------------------------------------------------------

        const CombatTarget& target =
            combat->target();

        const CombatStats stats =
            target.combatStats();

        // The current combat system uses 1 damage per enemy attack.
        // A target with zero attacks cannot damage the player.
        const int damage =
            stats.attacks > 0
            ? 1
            : 0;

        /*
        const int hp =
            target.currentHp();
        */
        const int hp =
            game.displayedCombatTargetHp();


        const EnemyStatSlot slots[] =
        {
            { "ATK", EnemyStatAtkY,   stats.attacks },
            { "STR", EnemyStatStrY,   stats.strength },
            { "PRE", EnemyStatPrecY,  stats.precision },
            { "DMG", EnemyStatDmgY,   damage },
            { "DEF", EnemyStatDefY,   stats.defense },
            { "TGH", EnemyStatToughY, stats.toughness },
            { "HP",  EnemyStatHpY,    hp }
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

            const Texture2D& numberTexture =
                m_mainScreenAssets.number(
                    slot.value);

            if (numberTexture.id != 0)
            {
                drawTexture(
                    numberTexture,
                    numberX,
                    slot.y,
                    StatNumberSize,
                    StatNumberSize);
            }
            else
            {
                drawFallbackText(
                    std::to_string(slot.value),
                    numberX,
                    slot.y,
                    StatNumberSize,
                    StatNumberSize,
                    18);
            }
        }
    }

    // =========================================================================
    // Player Gear Slot Fallback
    // =========================================================================

    void Renderer::drawGearSlots() const
    {
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

    // =========================================================================
    // Player Stat PNGs / Fallbacks
    // =========================================================================

    void Renderer::drawStatAssets(
        const Game& game) const
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
        /*
        const int hp =
            game.player().currentHp();
        */

        const int hp =
            game.displayedPlayerHp();

        // ---------------------------------------------------------------------
        // ATK
        // ---------------------------------------------------------------------

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

        // ---------------------------------------------------------------------
        // STR
        // ---------------------------------------------------------------------

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

        // ---------------------------------------------------------------------
        // PREC
        // ---------------------------------------------------------------------

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

        // ---------------------------------------------------------------------
        // DMG
        // ---------------------------------------------------------------------

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

        // ---------------------------------------------------------------------
        // DEF
        // ---------------------------------------------------------------------

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

        // ---------------------------------------------------------------------
        // TOUGH
        // ---------------------------------------------------------------------

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

        // ---------------------------------------------------------------------
        // HP
        // ---------------------------------------------------------------------

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

        // ---------------------------------------------------------------------
        // Numbers
        // ---------------------------------------------------------------------

        struct StatNumber
        {
            int value;
            int y;
        };

        const StatNumber numbers[] =
        {
            { attack,    StatAtkY },
            { strength,  StatStrY },
            { precision, StatPrecY },
            { damage,    StatDmgY },
            { defense,   StatDefY },
            { toughness, StatToughY },
            { hp,        StatHpY }
        };

        for (const StatNumber& stat : numbers)
        {
            const Texture2D& numberTexture =
                m_mainScreenAssets.number(
                    stat.value);

            if (numberTexture.id != 0)
            {
                drawTexture(
                    numberTexture,
                    StatNumberX,
                    stat.y,
                    StatNumberSize,
                    StatNumberSize);
            }
            else
            {
                drawFallbackText(
                    std::to_string(stat.value),
                    StatNumberX,
                    stat.y,
                    StatNumberSize,
                    StatNumberSize,
                    18);
            }
        }
    }

    // =========================================================================
    // Player Gear PNGs / Fallbacks
    // =========================================================================

    void Renderer::drawGearAssets(
        const Game& game) const
    {
        const Player& player =
            game.player();

        const Inventory& inventory =
            player.inventory();

        // ---------------------------------------------------------------------
        // Health Potion
        // ---------------------------------------------------------------------

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

        const int healthAmount =
            static_cast<int>(
                inventory.amount(
                    HealthPotion{}));

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

        // ---------------------------------------------------------------------
        // Rage Potion
        // ---------------------------------------------------------------------

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

        const int rageAmount =
            static_cast<int>(
                inventory.amount(
                    RagePotion{}));

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

        // ---------------------------------------------------------------------
        // Weapon
        // ---------------------------------------------------------------------

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
                    std::string("W") +
                    std::to_string(
                        weapon->tier()),
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

        // ---------------------------------------------------------------------
        // Armor
        // ---------------------------------------------------------------------

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
                    std::string("A") +
                    std::to_string(
                        armor->tier()),
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

        // ---------------------------------------------------------------------
        // Accessories
        // ---------------------------------------------------------------------

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

        // Tier 1 / Magic Skull
        if (hasMagicSkull &&
            m_mainScreenAssets.magicSkull().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.magicSkull(),
                Tier1AccessoryX,
                AccessoriesY,
                GearIconSize,
                GearIconSize);
        }
        else if (!hasMagicSkull &&
            m_mainScreenAssets.noAccessory().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.noAccessory(),
                Tier1AccessoryX,
                AccessoriesY,
                GearIconSize,
                GearIconSize);
        }
        else
        {
            drawFallbackText(
                hasMagicSkull ? "MS" : "--",
                Tier1AccessoryX,
                AccessoriesY,
                GearIconSize,
                GearIconSize,
                14);
        }

        // Tier 2 / Orc Fang
        if (hasOrcFang &&
            m_mainScreenAssets.orcFang().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.orcFang(),
                Tier2AccessoryX,
                AccessoriesY,
                GearIconSize,
                GearIconSize);
        }
        else if (!hasOrcFang &&
            m_mainScreenAssets.noAccessory().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.noAccessory(),
                Tier2AccessoryX,
                AccessoriesY,
                GearIconSize,
                GearIconSize);
        }
        else
        {
            drawFallbackText(
                hasOrcFang ? "OF" : "--",
                Tier2AccessoryX,
                AccessoriesY,
                GearIconSize,
                GearIconSize,
                14);
        }

        // Tier 3 / Troll's Heart
        if (hasTrollHeart &&
            m_mainScreenAssets.trollHeart().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.trollHeart(),
                Tier3AccessoryX,
                AccessoriesY,
                GearIconSize,
                GearIconSize);
        }
        else if (!hasTrollHeart &&
            m_mainScreenAssets.noAccessory().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.noAccessory(),
                Tier3AccessoryX,
                AccessoriesY,
                GearIconSize,
                GearIconSize);
        }
        else
        {
            drawFallbackText(
                hasTrollHeart ? "TH" : "--",
                Tier3AccessoryX,
                AccessoriesY,
                GearIconSize,
                GearIconSize,
                14);
        }
    }

    // =========================================================================
    // Traversal Action Bar
    // =========================================================================

    void Renderer::drawActionBar() const
    {
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

        drawFallbackText(
            "TRAVERSAL CONTROLS",
            ActionBarX,
            ActionBarY + 140,
            ActionBarWidth,
            40,
            22);

        drawFallbackText(
            "W / UP  -  MOVE UP",
            ActionBarX + 25,
            ActionBarY + 85,
            250,
            35,
            16);

        drawFallbackText(
            "S / DOWN  -  MOVE DOWN",
            ActionBarX + 275,
            ActionBarY + 85,
            250,
            35,
            16);

        drawFallbackText(
            "A / LEFT  -  MOVE LEFT",
            ActionBarX + 525,
            ActionBarY + 85,
            250,
            35,
            16);

        drawFallbackText(
            "D / RIGHT  -  MOVE RIGHT",
            ActionBarX + 775,
            ActionBarY + 85,
            250,
            35,
            16);

        drawFallbackText(
            "TAB  -  GAME INSTRUCTIONS",
            ActionBarX,
            ActionBarY + 25,
            ActionBarWidth,
            35,
            16);
    }

    // =========================================================================
    // Combat Action Bar
    // =========================================================================

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

        drawFallbackText(
            "COMBAT ACTIONS",
            ActionBarX,
            ActionBarY + 145,
            ActionBarWidth,
            35,
            22);

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

    // =========================================================================
    // Dice Roll
    // =========================================================================

    void Renderer::drawDiceRoll(
        const Game& game) const
    {
        const CombatPresentation& presentation =
            game.combatPresentation();

        constexpr int DiceWindowX = 230;
        constexpr int DiceWindowY = 260;
        constexpr int DiceWindowWidth = 240;
        constexpr int DiceWindowHeight = 360;

        constexpr int DieSize = 110;

        const int dieX =
            DiceWindowX +
            (DiceWindowWidth - DieSize) / 2;

        const int dieY =
            DiceWindowY +
            175;

        // -------------------------------------------------------------------------
        // Idle
        // -------------------------------------------------------------------------

        if (!presentation.active())
        {
            drawWrappedFallbackText(
                "Waiting for\nDice Action",
                DiceWindowX,
                dieY,
                DiceWindowWidth,
                DieSize,
                22,
                28);

            return;
        }

        // -------------------------------------------------------------------------
        // Information
        //
        // The player has selected an action, but the dice have not started rolling
        // yet. Keep the Combat Info message separate from this Dice Roll message.
        // -------------------------------------------------------------------------

        if (presentation.phase() ==
            CombatPresentation::Phase::Information)
        {
            drawWrappedFallbackText(
                "Preparing to\nRoll Dice",
                DiceWindowX,
                dieY,
                DiceWindowWidth,
                DieSize,
                22,
                28);

            return;
        }

        // -------------------------------------------------------------------------
        // Rolling
        // -------------------------------------------------------------------------

        if (presentation.phase() ==
            CombatPresentation::Phase::Rolling)
        {
            // Decorative only.
            // It has NO connection to the actual combat roll.
            const int dieValue =
                static_cast<int>(
                    presentation.phaseElapsed() *
                    10.0f)
                % 6 + 1;

            drawFallbackDie(
                dieValue,
                dieX,
                dieY,
                DieSize);

            return;
        }

        // -------------------------------------------------------------------------
        // Result
        // -------------------------------------------------------------------------

        if (presentation.phase() ==
            CombatPresentation::Phase::Result)
        {
            const AttackResult* result =
                presentation.currentAttack();

            if (result == nullptr)
            {
                return;
            }

            int dieValue = 1;

            switch (presentation.rollType())
            {
            case CombatPresentation::RollType::Precision:
                dieValue =
                    result->precisionRoll;
                break;

            case CombatPresentation::RollType::Wound:
                dieValue =
                    result->woundRoll;
                break;

            case CombatPresentation::RollType::Defense:
                dieValue =
                    result->defenseRoll;
                break;

            case CombatPresentation::RollType::None:
            default:
                return;
            }

            drawFallbackDie(
                dieValue,
                dieX,
                dieY,
                DieSize);
        }
    }

    // =========================================================================
    // Fallback Die
    // =========================================================================

    void Renderer::drawFallbackDie(
        int value,
        int x,
        int y,
        int size) const
    {
        if (value < 1 ||
            value > 6)
        {
            value = 1;
        }

        const Rectangle die =
            toRaylibRectangle(
                x,
                y,
                size,
                size);

        DrawRectangleRec(
            die,
            WHITE);

        DrawRectangleLinesEx(
            die,
            3.0f,
            BLACK);

        const float left =
            die.x +
            size * 0.25f;

        const float centerX =
            die.x +
            size * 0.5f;

        const float right =
            die.x +
            size * 0.75f;

        const float top =
            die.y +
            size * 0.25f;

        const float centerY =
            die.y +
            size * 0.5f;

        const float bottom =
            die.y +
            size * 0.75f;

        const float radius =
            size * 0.07f;

        auto drawPip =
            [radius](
                float px,
                float py)
            {
                DrawCircle(
                    static_cast<int>(px),
                    static_cast<int>(py),
                    radius,
                    BLACK);
            };

        switch (value)
        {
        case 1:
            drawPip(
                centerX,
                centerY);
            break;

        case 2:
            drawPip(left, top);
            drawPip(right, bottom);
            break;

        case 3:
            drawPip(left, top);
            drawPip(centerX, centerY);
            drawPip(right, bottom);
            break;

        case 4:
            drawPip(left, top);
            drawPip(right, top);
            drawPip(left, bottom);
            drawPip(right, bottom);
            break;

        case 5:
            drawPip(left, top);
            drawPip(right, top);
            drawPip(centerX, centerY);
            drawPip(left, bottom);
            drawPip(right, bottom);
            break;

        case 6:
            drawPip(left, top);
            drawPip(right, top);
            drawPip(left, centerY);
            drawPip(right, centerY);
            drawPip(left, bottom);
            drawPip(right, bottom);
            break;
        }
    }

    // =========================================================================
    // Combat Info
    // =========================================================================

    void Renderer::drawCombatInfo(
        const Game& game) const
    {
        const CombatPresentation& presentation =
            game.combatPresentation();

        constexpr int CombatInfoX = 230;
        constexpr int CombatInfoY = 260;
        constexpr int CombatInfoWidth = 240;
        constexpr int CombatInfoHeight = 100;

        // ---------------------------------------------------------------------
        // Idle
        // ---------------------------------------------------------------------

        if (!presentation.active())
        {
            drawWrappedFallbackText(
                "Select a Combat Action",
                CombatInfoX,
                CombatInfoY,
                CombatInfoWidth,
                CombatInfoHeight,
                16,
                22);

            return;
        }

        // ---------------------------------------------------------------------
        // Active presentation
        // ---------------------------------------------------------------------

        drawWrappedFallbackText(
            presentation.infoMessage(),
            CombatInfoX,
            CombatInfoY,
            CombatInfoWidth,
            CombatInfoHeight,
            16,
            22);
    }

    // =========================================================================
    // Map
    // =========================================================================

    void Renderer::drawMap(
        const Game& game) const
    {
        const Map& map =
            game.map();

        // ---------------------------------------------------------------------
        // Map Render
        // ---------------------------------------------------------------------

        const Rectangle mapRender =
            toRaylibRectangle(
                MapRenderX,
                MapRenderY,
                MapRenderWidth,
                MapRenderHeight);

        // Temporary frame fallback.
        if (m_mainScreenAssets.mapFrame().id == 0)
        {
            DrawRectangleRec(
                mapRender,
                DARKGRAY);
        }

        // ---------------------------------------------------------------------
        // Visible Map Area
        // ---------------------------------------------------------------------

        const int visibleMapX =
            MapRenderX +
            MapFrameThickness;

        const int visibleMapY =
            MapRenderY +
            MapFrameThickness;

        const Rectangle visibleMap =
            toRaylibRectangle(
                visibleMapX,
                visibleMapY,
                VisibleMapWidth,
                VisibleMapHeight);

        DrawRectangleRec(
            visibleMap,
            BLACK);

        // ---------------------------------------------------------------------
        // Map dimensions
        // ---------------------------------------------------------------------

        const int renderedMapWidth =
            static_cast<int>(
                map.width()) *
            TemporaryMapTileSize;

        const int renderedMapHeight =
            static_cast<int>(
                map.height()) *
            TemporaryMapTileSize;

        const int mapOffsetX =
            visibleMapX +
            (VisibleMapWidth -
                renderedMapWidth) / 2;

        const int mapOffsetY =
            visibleMapY +
            (VisibleMapHeight -
                renderedMapHeight) / 2;

        // ---------------------------------------------------------------------
        // Static Map Tiles
        // ---------------------------------------------------------------------

        for (std::size_t y = 0;
            y < map.height();
            ++y)
        {
            for (std::size_t x = 0;
                x < map.width();
                ++x)
            {
                const char tile =
                    map.tileAt(
                        static_cast<int>(x),
                        static_cast<int>(y));

                const int tileX =
                    mapOffsetX +
                    static_cast<int>(x) *
                    TemporaryMapTileSize;

                const int tileY =
                    mapOffsetY +
                    renderedMapHeight -
                    TemporaryMapTileSize -
                    static_cast<int>(y) *
                    TemporaryMapTileSize;

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

                if (texture != nullptr &&
                    texture->id != 0)
                {
                    drawTexture(
                        *texture,
                        tileX,
                        tileY,
                        TemporaryMapTileSize,
                        TemporaryMapTileSize);

                    continue;
                }

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

        const Player& player =
            game.player();

        const int playerX =
            mapOffsetX +
            player.x() *
            TemporaryMapTileSize;

        const int playerY =
            mapOffsetY +
            renderedMapHeight -
            TemporaryMapTileSize -
            player.y() *
            TemporaryMapTileSize;

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

        const auto& enemies =
            game.enemies();

        for (const auto& enemy : enemies)
        {
            const int enemyX =
                mapOffsetX +
                enemy->x() *
                TemporaryMapTileSize;

            const int enemyY =
                mapOffsetY +
                renderedMapHeight -
                TemporaryMapTileSize -
                enemy->y() *
                TemporaryMapTileSize;

            const std::string_view type =
                enemy->type();

            const Texture2D* enemyTexture =
                &m_mainScreenAssets.goblinTile();

            const char* identifier =
                "G";

            if (type == "Skeleton")
            {
                enemyTexture =
                    &m_mainScreenAssets.skeletonTile();

                identifier = "S";
            }
            else if (type == "Orc")
            {
                enemyTexture =
                    &m_mainScreenAssets.orcTile();

                identifier = "O";
            }
            else if (type == "Troll")
            {
                enemyTexture =
                    &m_mainScreenAssets.trollTile();

                identifier = "T";
            }
            else if (type == "Dragon")
            {
                enemyTexture =
                    &m_mainScreenAssets.dragonTile();

                identifier = "D";
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

        const auto& chests =
            game.chests();

        for (const auto& chest : chests)
        {
            const int chestX =
                mapOffsetX +
                chest->x() *
                TemporaryMapTileSize;

            const int chestY =
                mapOffsetY +
                renderedMapHeight -
                TemporaryMapTileSize -
                chest->y() *
                TemporaryMapTileSize;

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

                drawFallbackText(
                    "C",
                    chestX,
                    chestY,
                    TemporaryMapTileSize,
                    TemporaryMapTileSize,
                    11);
            }
        }

        // ---------------------------------------------------------------------
        // Map Frame
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.mapFrame().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.mapFrame(),
                MapRenderX,
                MapRenderY,
                MapRenderWidth,
                MapRenderHeight);
        }
        else
        {
            DrawRectangleLinesEx(
                mapRender,
                static_cast<float>(
                    MapFrameThickness),
                DARKGRAY);
        }
    }
}