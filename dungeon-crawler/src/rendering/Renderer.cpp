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

#include "loot/LootReward.h"
#include "inventory/HealthPotion.h"
#include "inventory/RagePotion.h"

#include <algorithm>
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

            if (tile == '@')
            {
                return &assets.portalTile();
            }

            return &assets.floorTile();
        }



        // Enemy Loot render
        const Texture2D* lootTextureForPreview(
            const MainScreenAssets& assets,
            const CombatLootPreviewSlot& slot)
        {
            switch (slot.type)
            {
            case CombatLootPreviewType::Weapon:
                return &assets.weaponTier(slot.tier);

            case CombatLootPreviewType::Armor:
                return &assets.armorTier(slot.tier);

            case CombatLootPreviewType::Accessory:
                if (slot.id == "magic_skull")
                {
                    return &assets.magicSkull();
                }

                if (slot.id == "orc_fang")
                {
                    return &assets.orcFang();
                }

                if (slot.id == "troll_heart")
                {
                    return &assets.trollHeart();
                }

                if (slot.id == "no_accessory")
                {
                    return &assets.noAccessory();
                }

                break;

            case CombatLootPreviewType::HealthPotion:
                return &assets.healthPotionIcon();

            case CombatLootPreviewType::RagePotion:
                return &assets.ragePotionIcon();

            case CombatLootPreviewType::None:
                break;
            }

            return nullptr;
        }



        std::string lootFallbackLabel(
            const CombatLootPreviewSlot& slot)
        {
            switch (slot.type)
            {
            case CombatLootPreviewType::Weapon:
                return "W" + std::to_string(slot.tier);

            case CombatLootPreviewType::Armor:
                return "A" + std::to_string(slot.tier);

            case CombatLootPreviewType::Accessory:
                return "ACC";

            case CombatLootPreviewType::HealthPotion:
                return "HP";

            case CombatLootPreviewType::RagePotion:
                return "RAGE";

            case CombatLootPreviewType::None:
                return "None";
            }

            return "None";
        }

        enum class CombatPovFallbackState
        {
            Idle,
            Hurt,
            Attack,
            Death
        };

        std::string combatEnemyFallbackText(
            std::string_view enemyType,
            CombatPovFallbackState state)
        {
            if (enemyType == "Goblin")
            {
                switch (state)
                {
                case CombatPovFallbackState::Idle:
                    return "The Goblin watches you from the darkness.";

                case CombatPovFallbackState::Hurt:
                    return "The Goblin recoils from your attack.";

                case CombatPovFallbackState::Attack:
                    return "The Goblin lunges at you.";

                case CombatPovFallbackState::Death:
                    return "The Goblin falls defeated.";
                }
            }

            if (enemyType == "Skeleton")
            {
                switch (state)
                {
                case CombatPovFallbackState::Idle:
                    return "The Skeleton stands silently before you.";

                case CombatPovFallbackState::Hurt:
                    return "The Skeleton staggers from your attack.";

                case CombatPovFallbackState::Attack:
                    return "The Skeleton strikes at you with its weapon.";

                case CombatPovFallbackState::Death:
                    return "The Skeleton collapses into a heap of bones.";
                }
            }

            if (enemyType == "Orc")
            {
                switch (state)
                {
                case CombatPovFallbackState::Idle:
                    return "The Orc grips its Axe and glares at you.";

                case CombatPovFallbackState::Hurt:
                    return "The Orc reels from your attack.";

                case CombatPovFallbackState::Attack:
                    return "The Orc charges forward and attacks.";

                case CombatPovFallbackState::Death:
                    return "The Orc crashes to the ground, defeated.";
                }
            }

            if (enemyType == "Troll")
            {
                switch (state)
                {
                case CombatPovFallbackState::Idle:
                    return "The Troll towers over you.";

                case CombatPovFallbackState::Hurt:
                    return "The Troll roars as your attack wounds it.";

                case CombatPovFallbackState::Attack:
                    return "The Troll swings with terrible force.";

                case CombatPovFallbackState::Death:
                    return "The Troll finally falls before you.";
                }
            }

            if (enemyType == "Dragon")
            {
                switch (state)
                {
                case CombatPovFallbackState::Idle:
                    return "The Dragon watches you through the darkness.";

                case CombatPovFallbackState::Hurt:
                    return "The Dragon recoils suprised from your attack.";

                case CombatPovFallbackState::Attack:
                    return "The Dragon lunges toward you.";

                case CombatPovFallbackState::Death:
                    return "The Dragon collapses, defeated.";
                }
            }

            switch (state)
            {
            case CombatPovFallbackState::Idle:
                return std::string(enemyType) +
                    " stands before you.";

            case CombatPovFallbackState::Hurt:
                return std::string(enemyType) +
                    " recoils from your attack.";

            case CombatPovFallbackState::Attack:
                return std::string(enemyType) +
                    " attacks you.";

            case CombatPovFallbackState::Death:
                return std::string(enemyType) +
                    " falls defeated.";
            }

            return std::string(enemyType);
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

    void Renderer::drawTextureRotated(
        const Texture2D& texture,
        int x,
        int y,
        int width,
        int height,
        float rotation) const
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

        const Vector2 origin{
            static_cast<float>(width) / 2.0f,
            static_cast<float>(height) / 2.0f
        };

        DrawTexturePro(
            texture,
            source,
            destination,
            origin,
            rotation,
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
    // Instruction Page
    //
    // Layered composition:
    //   1. Background layer  - background texture, or fallback panel
    //   2. Foreground layer  - complete instructions image if present,
    //                          otherwise raylib title/body fallback text
    //
    // The complete image and the fallback text are mutually exclusive;
    // the background (or its fallback panel) is always drawn underneath
    // whichever foreground is used.
    // =========================================================================

    void Renderer::drawInstructionPage(
        const Texture2D& background,
        const Texture2D& complete,
        std::string_view title,
        std::string_view body,
        int fontSize,
        int lineSpacing) const
    {
        const Rectangle pov =
            toRaylibRectangle(
                PovRenderX,
                PovRenderY,
                PovRenderWidth,
                PovRenderHeight);

        // ---------------------------------------------------------------------
        // Step 1: Background layer
        // ---------------------------------------------------------------------

        if (background.id != 0)
        {
            drawTexture(
                background,
                PovRenderX,
                PovRenderY,
                PovRenderWidth,
                PovRenderHeight);
        }
        else
        {
            DrawRectangleRec(pov, LIGHTGRAY);
            DrawRectangleLinesEx(pov, 1.0f, DARKGRAY);
        }

        // ---------------------------------------------------------------------
        // Step 2: Foreground layer
        // ---------------------------------------------------------------------

        if (complete.id != 0)
        {
            drawTexture(
                complete,
                PovRenderX,
                PovRenderY,
                PovRenderWidth,
                PovRenderHeight);

            return;
        }

        drawFallbackText(
            title,
            PovRenderX + 20,
            PovRenderY + PovRenderHeight - 58,
            PovRenderWidth - 40,
            38,
            24);

        drawWrappedFallbackText(
            body,
            PovRenderX + 28,
            PovRenderY + 28,
            PovRenderWidth - 56,
            PovRenderHeight - 96,
            fontSize,
            lineSpacing);
    }

    // =========================================================================
    // Main Renderer
    // =========================================================================

    void Renderer::draw(
        const Game& game) const
    {
        ClearBackground(RAYWHITE);

        if (game.inCombat() || game.defeatPromptActive())
        {
            drawCombatScreenLayout(game);
        }
        else
        {
            drawMainScreenLayout(game);
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

        if (game.defeatPromptActive())
        {
            drawEnemyStatSlots(game);
            drawLootTable(game);
            drawDiceRoll(game);
            drawCombatInfo(game);
            drawCombatPov(game);
            drawDefeatedActionBar();
        }
        else if (game.inCombat())
        {
            drawEnemyStatSlots(game);
            drawLootTable(game);
            drawDiceRoll(game);
            drawCombatInfo(game);
            drawCombatPov(game);

            if (game.inventoryOpen())
            {
                drawInventoryActionBar();
            }
            else
            {
                drawCombatActionBar();
            }
        }
        else
        {
            drawMap(game);

            if (game.portalPromptActive())
            {
                drawPortalActionBar();
            }
            else
            {
                drawActionBar();
            }

            drawTraversalPov(game);
        }
    }

    // =========================================================================
    // Main Screen Layout
    // =========================================================================

    void Renderer::drawMainScreenLayout(const Game& game) const
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

            DrawRectangleRec(titleArt, LIGHTGRAY);
            DrawRectangleLinesEx(titleArt, 1.0f, DARKGRAY);
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

            DrawRectangleRec(statsTitle, LIGHTGRAY);
            DrawRectangleLinesEx(statsTitle, 1.0f, DARKGRAY);
            drawFallbackText(
                "Player Stats",
                StatsTitleX,
                StatsTitleY,
                StatsTitleWidth,
                StatsTitleHeight,
                22);
        }

        // ---------------------------------------------------------------------
        // Player Stats Window Background
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.playerStatsWindowBackground().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.playerStatsWindowBackground(),
                StatsWindowX,
                StatsWindowY,
                StatsWindowWidth,
                StatsWindowHeight);
        }
        else
        {
            const Rectangle statsWindow =
                toRaylibRectangle(
                    StatsWindowX,
                    StatsWindowY,
                    StatsWindowWidth,
                    StatsWindowHeight);

            DrawRectangleRec(statsWindow, RAYWHITE);
            DrawRectangleLinesEx(statsWindow, 1.0f, DARKGRAY);
        }

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

            DrawRectangleRec(gearTitle, LIGHTGRAY);
            DrawRectangleLinesEx(gearTitle, 1.0f, DARKGRAY);
            drawFallbackText(
                "Player Gear",
                GearTitleX,
                GearTitleY,
                GearTitleWidth,
                GearTitleHeight,
                22);
        }

        // ---------------------------------------------------------------------
        // Gear Window Background
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.gearWindowBackground().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.gearWindowBackground(),
                GearWindowX,
                GearWindowY,
                GearWindowWidth,
                GearWindowHeight);
        }
        else
        {
            const Rectangle gearWindow =
                toRaylibRectangle(
                    GearWindowX,
                    GearWindowY,
                    GearWindowWidth,
                    GearWindowHeight);

            DrawRectangleRec(gearWindow, RAYWHITE);
            DrawRectangleLinesEx(gearWindow, 1.0f, DARKGRAY);
        }

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

            DrawRectangleRec(actionBar, LIGHTGRAY);
            DrawRectangleLinesEx(actionBar, 1.0f, DARKGRAY);
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

        if (m_gameInstructionsOpen) //----------------------------------------------- Travarsal INSTRUCTIONS HERE
        {
            drawInstructionPage(
                m_mainScreenAssets.gameInstructionsBackground(),
                m_mainScreenAssets.gameInstructions(),//------------------- THis looks Good
                "GAME INSTRUCTIONS",
                "\n"
                "----------------------------------------------\n"
                "\n"
                "MOVEMENT\n"
                "W / UP       Move Up\n"
                "S / DOWN     Move Down\n"
                "A / LEFT     Move Left\n"
                "D / RIGHT    Move Right\n"
                "\n"
                "----------------------------------------------\n"
                "\n"
                "TURNING\n"
                "Q            Turn Left\n"
                "E            Turn Right\n"
                "\n"
                "----------------------------------------------\n"
                "\n"
                "ENCOUNTERS\n"
                "Enemy        May Grant you Gear\n"
                "Chest        May Grant you Potions\n"
                "Portal       is Your escape From The Dungeon\n"
                "\n"
                "----------------------------------------------\n"
                "\n"
                "OBJECTIVE\n"
                "Explore the dungeon,\n"
                "defeat enemies,\n"
                "collect Loot, \n"
                "and find the Portal.\n"
                "\n"
                "\n"
                "----------------------------------------------\n"
                "\n"
                "\n"
                "TAB          Close Instructions",
                16,
                18);
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
    }

    // =========================================================================
    // Combat Screen Layout
    // =========================================================================
    void Renderer::drawCombatScreenLayout(const Game& game) const
    {
        // ---------------------------------------------------------------------
        // Player Stats Window Background
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.playerStatsWindowBackground().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.playerStatsWindowBackground(),
                StatsWindowX,
                StatsWindowY,
                StatsWindowWidth,
                StatsWindowHeight);
        }
        else
        {
            const Rectangle statsWindow =
                toRaylibRectangle(
                    StatsWindowX,
                    StatsWindowY,
                    StatsWindowWidth,
                    StatsWindowHeight);
            DrawRectangleRec(statsWindow, RAYWHITE);
            DrawRectangleLinesEx(statsWindow, 1.0f, DARKGRAY);
        }

        // ---------------------------------------------------------------------
        // Gear Window Background
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.gearWindowBackground().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.gearWindowBackground(),
                GearWindowX,
                GearWindowY,
                GearWindowWidth,
                GearWindowHeight);
        }
        else
        {
            const Rectangle gearWindow =
                toRaylibRectangle(
                    GearWindowX,
                    GearWindowY,
                    GearWindowWidth,
                    GearWindowHeight);
            DrawRectangleRec(gearWindow, RAYWHITE);
            DrawRectangleLinesEx(gearWindow, 1.0f, DARKGRAY);
        }

        // ---------------------------------------------------------------------
        // Enemy Stats Title / Background
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.enemyStatsTitle().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.enemyStatsTitle(),
                0,
                620,
                230,
                50);
        }
        else
        {
            const Rectangle title =
                toRaylibRectangle(
                    0,
                    620,
                    230,
                    50);

            DrawRectangleRec(
                title,
                LIGHTGRAY);

            DrawRectangleLinesEx(
                title,
                1.0f,
                DARKGRAY);

            drawFallbackText(
                "Enemy Stats",
                0,
                620,
                230,
                50,
                20);
        }

        if (m_mainScreenAssets.enemyStatsWindowBackground().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.enemyStatsWindowBackground(),
                0,
                260,
                230,
                360);
        }
        else
        {
            const Rectangle enemyStatsWindow =
                toRaylibRectangle(
                    0,
                    260,
                    230,
                    360);
            DrawRectangleRec(enemyStatsWindow, RAYWHITE);
            DrawRectangleLinesEx(enemyStatsWindow, 1.0f, DARKGRAY);
        }

        // ---------------------------------------------------------------------
        // Dice Roll Title / Background
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.diceRollTitle().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.diceRollTitle(),
                230,
                620,
                240,
                50);
        }
        else
        {
            const Rectangle title =
                toRaylibRectangle(
                    230,
                    620,
                    240,
                    50);

            DrawRectangleRec(
                title,
                LIGHTGRAY);

            DrawRectangleLinesEx(
                title,
                1.0f,
                DARKGRAY);

            drawFallbackText(
                "Dice Roll",
                230,
                620,
                240,
                50,
                20);
        }

        if (m_mainScreenAssets.diceRollWindowBackground().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.diceRollWindowBackground(),
                230,
                260,
                240,
                360);
        }
        else
        {
            const Rectangle diceRollWindow =
                toRaylibRectangle(
                    230,
                    260,
                    240,
                    360);
            DrawRectangleRec(diceRollWindow, GRAY);
            DrawRectangleLinesEx(diceRollWindow, 1.0f, DARKGRAY);
        }

        // ---------------------------------------------------------------------
        // Combat Info Window Background
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.combatInfoWindowBackground().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.combatInfoWindowBackground(),
                230,
                260,
                240,
                100);
        }
        else
        {
            const Rectangle combatInfoWindow =
                toRaylibRectangle(
                    230,
                    260,
                    240,
                    100);
            DrawRectangleRec(combatInfoWindow, RAYWHITE);
            DrawRectangleLinesEx(combatInfoWindow, 1.0f, DARKGRAY);
        }

        // ---------------------------------------------------------------------
        // Loot Title / Table Background
        // ---------------------------------------------------------------------

        if (m_mainScreenAssets.lootTitle().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.lootTitle(),
                0,
                200,
                104,
                60);
        }
        else
        {
            const Rectangle title =
                toRaylibRectangle(
                    0,
                    200,
                    104,
                    60);

            DrawRectangleRec(
                title,
                LIGHTGRAY);

            DrawRectangleLinesEx(
                title,
                1.0f,
                DARKGRAY);

            drawFallbackText(
                "Loot",
                0,
                200,
                104,
                60,
                18);
        }

        if (m_mainScreenAssets.lootTableWindowBackground().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.lootTableWindowBackground(),
                104,
                200,
                366,
                60);
        }
        else
        {
            const Rectangle lootTableWindow =
                toRaylibRectangle(
                    104,
                    200,
                    366,
                    60);

            DrawRectangleRec(
                lootTableWindow,
                RAYWHITE);

            DrawRectangleLinesEx(
                lootTableWindow,
                1.0f,
                DARKGRAY);
        }

        // ---------------------------------------------------------------------
        // POV / Instructions
        // ---------------------------------------------------------------------

        const Rectangle povRender =
            toRaylibRectangle(
                PovRenderX,
                PovRenderY,
                PovRenderWidth,
                PovRenderHeight);

        if (m_gameInstructionsOpen)
        {
            if (game.inventoryOpen())  //------------------------------------------------------------------- Inventory INSTRUCTIONS HERE!
            {
                drawInstructionPage(
                    m_mainScreenAssets.inventoryInstructionsBackground(),
                    m_mainScreenAssets.inventoryInstructions(),//---------------------------- This Looks acceptable
                    
                    //"INVENTORY INSTRUCTIONS",
                    //"----------------------------------------------",
                    "                                                 ",
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "HEALTH POTION\n"
                    "Restores up to 3 HP.\n"
                    "It Cannot be used at full HP.\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "RAGE POTION\n"
                    "Adds +1 Damage, Rage bonuses stack.\n"
                    "Lasts for the current combat.\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "BEWARE!\n"
                    "Using a potion ends your turn.\n"
                    "And The enemy attacks next.\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "GEAR\n"
                    "\n"
                    "WEAPONS\n"
                    "Higher Tier Weapons provide\n"
                    "greater Strength Stat Value.\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "ARMOR\n"
                    "Higher Tier Armor provides\n"
                    "Higher Defense Stat value.\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "ACCESSORIES\n"
                    "Can provide bonuses to\n"
                    "Attacks, Precision, or Maximum HP.\n"
                    "\n"
                    /*
                    "\n"
                    "ACTIONS\n"
                    "1            Health Potion\n"
                    "2            Rage Potion\n"
                    "3            Close Inventory\n"
                    "\n"
                    "----------------------------------------------\n"
                    */
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "\n"
                    "TAB          Close Instructions",
                    14,
                    16);


                /*
                //"INVENTORY INSTRUCTIONS",
                //"----------------------------------------------",
                "                                                 ",
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "HEALTH POTION\n"
                    "Restores up to 3 HP.\n"
                    "It Cannot be used at full HP.\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "RAGE POTION\n"
                    "Adds +1 Damage, Rage bonuses stack.\n"
                    "Lasts for the current combat.\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "GEAR\n"
                    "\n"
                    "WEAPONS\n"
                    "Higher Tier Weapons provide\n"
                    "greater Strength Stat Value.\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "ARMOR\n"
                    "Higher Tier Armor provides\n"
                    "Higher Defense Stat value.\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "ACCESSORIES\n"
                    "Can provide bonuses to\n"
                    "Attacks, Precision, or Maximum HP.\n"
                    "\n"
                    "----------------------------------------------\n"
                    
                    //"\n"
                    //"ACTIONS\n"
                    //"1            Health Potion\n"
                    //"2            Rage Potion\n"
                    //"3            Close Inventory\n"
                    //"\n"
                    //"----------------------------------------------\n"
                    
                    "\n"
                    "BEWARE!\n"
                    "Using a potion ends your turn.\n"
                    "And The enemy attacks next.\n"
                    "\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "\n"
                    "TAB          Close Instructions",
                    14,
                    16);
                */

            }
            else
            {
                drawInstructionPage(
                    m_mainScreenAssets.combatInstructionsBackground(),
                    m_mainScreenAssets.combatInstructions(), //--------------------------------------------------------- Combat INSTRUCTIONS HERE!
                    //----------------------------------------- This Looks accaptable
                    //"COMBAT INSTRUCTIONS",
                    //"----------------------------------------------",
                    "      ",
                    /*
                    "ACTIONS\n"
                    "F1 / 1       Attack\n"
                    "I / 2        Open Inventory\n"
                    "E / 3        Escape\n"
                    "\n"
                    "----------------------------------------------\n"
                    */
                    "\n"
                    "ATTACK\n"
                    "ATK = Number of Attacks\n"
                    "Each attack uses a D6.\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "PRECISION\n"
                    "D6 >= Precision\n"
                    "Fail = Miss\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "WOUND\n"
                    "Compare Strength to Toughness.\n"
                    "\n"
                    "TGH <= 0         Target 2\n"
                    "STR >= TGH x 2   Target 2\n"
                    "STR > TGH        Target 3\n"
                    "STR = TGH        Target 4\n"
                    "STR x 2 <= TGH   Target 6\n"
                    "Otherwise        Target 5\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "DEFENSE\n"
                    "D6 >= Defense\n"
                    "Pass = 0 Damage\n"
                    "Fail = Damage Applied\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "DAMAGE\n"
                    "Damage is based on the Damage Stat\n"
                    "for the Player it's Weapon Damage + Rage Bonus\n"
                    "When 0 HP, the entity is Defeated\n"
                    "\n"
                    "\n"
                    //"----------------------------------------------\n"
                    "\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "\n"
                    "TAB          Close Instructions",
                    13,
                    15);


                /*
                                    //"COMBAT INSTRUCTIONS",
                    "----------------------------------------------",
                    "ACTIONS\n"
                    "F1 / 1       Attack\n"
                    "I / 2        Open Inventory\n"
                    "E / 3        Escape\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "ATTACK\n"
                    "Each attack uses a D6.\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "PRECISION\n"
                    "D6 >= Precision\n"
                    "Fail = Miss\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "WOUND\n"
                    "Compare Strength to Toughness.\n"
                    "\n"
                    "TGH <= 0         Target 2\n"
                    "STR >= TGH x 2   Target 2\n"
                    "STR > TGH        Target 3\n"
                    "STR = TGH        Target 4\n"
                    "STR x 2 <= TGH   Target 6\n"
                    "Otherwise        Target 5\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "DEFENSE\n"
                    "D6 >= Defense\n"
                    "Pass = 0 Damage\n"
                    "Fail = Damage Applied\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "DAMAGE\n"
                    "Weapon Damage + Rage Bonus\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "ATK = Number of Attacks\n"
                    "0 HP = Defeated\n"
                    "\n"
                    "\n"
                    "----------------------------------------------\n"
                    "\n"
                    "\n"
                    "TAB          Close Instructions",
                    13,
                    15);
                */
            }
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
    }

    void Renderer::drawLootTable(
        const Game& game) const
    {
        constexpr int LootIconSize = 48;
        constexpr int LootIconY = 206;

        constexpr int LootIconXs[] =
        {
            125,
            194,
            263,
            332,
            401
        };

        const auto& preview =
            game.combatLootPreview();

        for (std::size_t index = 0; index < 5; ++index)
        {
            const CombatLootPreviewSlot* slot = nullptr;

            if (index < preview.size())
            {
                slot = &preview[index];
            }

            const CombatLootPreviewSlot emptySlot{};

            const CombatLootPreviewSlot& currentSlot =
                slot != nullptr
                ? *slot
                : emptySlot;

            const Rectangle lootSlot =
                toRaylibRectangle(
                    LootIconXs[index],
                    LootIconY,
                    LootIconSize,
                    LootIconSize);

            DrawRectangleLinesEx(
                lootSlot,
                1.0f,
                GRAY);

            if (currentSlot.type == CombatLootPreviewType::None)
            {
                drawFallbackText(
                    "None",
                    LootIconXs[index],
                    LootIconY,
                    LootIconSize,
                    LootIconSize,
                    10);

                continue;
            }

            const Texture2D* texture =
                lootTextureForPreview(
                    m_mainScreenAssets,
                    currentSlot);

            if (texture != nullptr &&
                texture->id != 0)
            {
                drawTexture(
                    *texture,
                    LootIconXs[index],
                    LootIconY,
                    LootIconSize,
                    LootIconSize);

                continue;
            }

            drawFallbackText(
                lootFallbackLabel(currentSlot),
                LootIconXs[index],
                LootIconY,
                LootIconSize,
                LootIconSize,
                10);
        }
    }


    // =========================================================================
    // Titles
    // =========================================================================

    void Renderer::drawTitleAssets() const
    {
        // -------------------------------------------------------------------------
        // Title Art
        // -------------------------------------------------------------------------

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

        // -------------------------------------------------------------------------
        // Stats Title
        // -------------------------------------------------------------------------

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

        // -------------------------------------------------------------------------
        // Gear Title
        // -------------------------------------------------------------------------

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

        if (m_mainScreenAssets.playerStatsWindowBackground().id == 0)
        {
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
    }

    // =========================================================================
    // Enemy Stats Slot Fallback
    // =========================================================================

    void Renderer::drawEnemyStatSlots(
        const Game& game) const
    {
        if (m_mainScreenAssets.enemyStatsWindowBackground().id != 0)
        {
            return;
        }
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
                const bool threshold =
                    slot.y == EnemyStatPrecY ||
                    slot.y == EnemyStatDefY;

                drawFallbackText(
                    threshold && slot.value > 0
                    ? std::to_string(slot.value) + "+"
                    : std::to_string(slot.value),
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
        if (m_mainScreenAssets.gearWindowBackground().id != 0)
        {
            return;
        }
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
            game.player().weaponDamage() +
            (game.combat() != nullptr
                ? game.combat()->playerDamageBonus()
                : 0);
        const int defense =
            stats.defense;

        const int toughness =
            stats.toughness;
        /*
        const int hp =
            game.displayedPlayerHp();
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
            const bool threshold =
                stat.y == StatPrecY ||
                stat.y == StatDefY;
            if (threshold)
            {
                drawFallbackText(
                    stat.value > 0
                    ? std::to_string(stat.value) + "+"
                    : "--",
                    StatNumberX,
                    stat.y,
                    StatNumberSize,
                    StatNumberSize,
                    18);
                continue;
            }
            const Texture2D& numberTexture =
                m_mainScreenAssets.number(stat.value);
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
            "Q  -  TURN LEFT",
            ActionBarX + 275,
            ActionBarY + 45,
            250,
            30,
            15);

        drawFallbackText(
            "E  -  TURN RIGHT",
            ActionBarX + 525,
            ActionBarY + 45,
            250,
            30,
            15);

        drawFallbackText(
            "TAB  -  GAME INSTRUCTIONS",
            ActionBarX,
            ActionBarY + 25,
            ActionBarWidth,
            35,
            16);
    }

    // =========================================================================
    // Traversal Portal Action Bar
    // =========================================================================
    void Renderer::drawPortalActionBar() const
    {
        if (m_mainScreenAssets.portalActionBar().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.portalActionBar(),
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

        drawWrappedFallbackText(
            "You Have Found the Portal Out of the Dungeon",
            ActionBarX + 25,
            ActionBarY + 112,
            600,
            45,
            18,
            22);

        drawWrappedFallbackText(
            "Go through and Exit the Dungeon?",
            ActionBarX + 25,
            ActionBarY + 62,
            600,
            40,
            18,
            22);

        drawFallbackText(
            "Y - EXIT",
            ActionBarX + 680,
            ActionBarY + 72,
            150,
            40,
            18);

        drawFallbackText(
            "N - STAY",
            ActionBarX + 850,
            ActionBarY + 72,
            150,
            40,
            18);
    }

    // =========================================================================
    // Combat Action Bar
    // =========================================================================

    void Renderer::drawCombatActionBar() const
    {
        if (m_mainScreenAssets.combatActionBar().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.combatActionBar(),
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
    // Defeat in Combat Action Bar
    // =========================================================================

    void Renderer::drawDefeatedActionBar() const
    {
        if (m_mainScreenAssets.defeatedActionBar().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.defeatedActionBar(),
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

        drawWrappedFallbackText(
            "You Have Defeated and lost forever in the Dungeon",
            ActionBarX + 25,
            ActionBarY + 112,
            700,
            45,
            18,
            22);

        drawWrappedFallbackText(
            "Rise back up and find the exit of the Dungeon?",
            ActionBarX + 25,
            ActionBarY + 62,
            700,
            40,
            18,
            22);

        drawFallbackText(
            "Y - RESTART",
            ActionBarX + 755,
            ActionBarY + 72,
            135,
            40,
            18);

        drawFallbackText(
            "N - CLOSE",
            ActionBarX + 900,
            ActionBarY + 72,
            125,
            40,
            18);
    }


    // =========================================================================
    // Inventory Action Bar
    // =========================================================================

    void Renderer::drawInventoryActionBar() const
    {
        if (m_mainScreenAssets.inventoryActionBar().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.inventoryActionBar(),
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

        DrawRectangleRec(actionBar, LIGHTGRAY);
        DrawRectangleLinesEx(actionBar, 1.0f, DARKGRAY);
        drawFallbackText("INVENTORY ACTIONS", ActionBarX, ActionBarY + 145, ActionBarWidth, 35, 22);
        drawFallbackText("1  -  HEALTH POTION", ActionBarX + 25, ActionBarY + 85, 250, 35, 17);
        drawFallbackText("2  -  RAGE POTION", ActionBarX + 275, ActionBarY + 85, 250, 35, 17);
        drawFallbackText("3  -  CLOSE INVENTORY", ActionBarX + 525, ActionBarY + 85, 250, 35, 17);
        drawFallbackText("TAB  -  GAME INVENTORY INSTRUCTIONS", ActionBarX, ActionBarY + 25, ActionBarWidth, 35, 17);
    }

    // =========================================================================
    // Traversal POV
    // =========================================================================
    void Renderer::drawTraversalPov(
        const Game& game) const
    {
        if (m_gameInstructionsOpen)
        {
            return;
        }

        const Player& player =
            game.player();

        int lookX =
            player.x();
        int lookY =
            player.y();

        switch (game.facingDirection())
        {
        case FacingDirection::North:
            --lookY;
            break;

        case FacingDirection::East:
            ++lookX;
            break;

        case FacingDirection::South:
            ++lookY;
            break;

        case FacingDirection::West:
            --lookX;
            break;
        }

        for (const auto& enemy : game.enemies())
        {
            if (!enemy ||
                enemy->x() != lookX ||
                enemy->y() != lookY)
            {
                continue;
            }

            const std::string_view enemyType =
                enemy->type();

            const Texture2D& texture =
                enemy->isDefeated()
                ? m_mainScreenAssets.seesDefeatedEnemy(
                    enemyType)
                : m_mainScreenAssets.seesEnemy(
                    enemyType);

            if (texture.id != 0)
            {
                drawTexture(
                    texture,
                    PovRenderX,
                    PovRenderY,
                    PovRenderWidth,
                    PovRenderHeight);
            }
            else
            {
                drawFallbackText(
                    enemy->isDefeated()
                    ? "A Defeated " +
                    std::string(enemyType) +
                    " Lays down before you"
                    : std::string(enemyType) +
                    " Stands in the Darkness",
                    PovRenderX,
                    PovRenderY + 210,
                    PovRenderWidth,
                    50,
                    28);
            }

            return;
        }

        for (const auto& chest : game.chests())
        {
            if (!chest ||
                chest->x() != lookX ||
                chest->y() != lookY)
            {
                continue;
            }

            const Texture2D& texture =
                chest->isDefeated()
                ? m_mainScreenAssets.seesDefeatedChest()
                : m_mainScreenAssets.seesChest();

            if (texture.id != 0)
            {
                drawTexture(
                    texture,
                    PovRenderX,
                    PovRenderY,
                    PovRenderWidth,
                    PovRenderHeight);
            }
            else
            {
                drawFallbackText(
                    chest->isDefeated()
                    ? "You See an Opened Chest in the Darkness"
                    : "You See an closed Chest in the Darkness",
                    PovRenderX,
                    PovRenderY + 210,
                    PovRenderWidth,
                    50,
                    28);
            }

            return;
        }

        if (!game.map().isWalkable(lookX, lookY))
        {
            if (m_mainScreenAssets.seesWall().id != 0)
            {
                drawTexture(
                    m_mainScreenAssets.seesWall(),
                    PovRenderX,
                    PovRenderY,
                    PovRenderWidth,
                    PovRenderHeight);
            }
            else
            {
                drawFallbackText(
                    "You facing Wall of the Dungeon",
                    PovRenderX,
                    PovRenderY + 210,
                    PovRenderWidth,
                    50,
                    28);
            }

            return;
        }

        if (m_mainScreenAssets.seesNothing().id != 0)
        {
            drawTexture(
                m_mainScreenAssets.seesNothing(),
                PovRenderX,
                PovRenderY,
                PovRenderWidth,
                PovRenderHeight);
        }
        else
        {
            drawFallbackText(
                "You See Nothing in the Darkness Ahead",
                PovRenderX,
                PovRenderY + 210,
                PovRenderWidth,
                50,
                24);
        }
    }

    // =========================================================================
    // Combat POV
    // =========================================================================
    void Renderer::drawCombatPov(
        const Game& game) const
    {
        if (m_gameInstructionsOpen)
        {
            return;
        }

        if (game.defeatPromptActive())
        {
            const Texture2D& texture =
                m_mainScreenAssets.enemyVictory(
                    game.defeatedEnemyType());

            if (texture.id != 0)
            {
                drawTexture(
                    texture,
                    PovRenderX,
                    PovRenderY,
                    PovRenderWidth,
                    PovRenderHeight);
            }
            else
            {
                std::string victoryText;

                if (game.defeatedEnemyType() == "Goblin")
                {
                    victoryText =
                        "The Goblin stands victorious over you.";
                }
                else if (game.defeatedEnemyType() == "Skeleton")
                {
                    victoryText =
                        "The Skeleton remains standing as you fall.";
                }
                else if (game.defeatedEnemyType() == "Orc")
                {
                    victoryText =
                        "The Orc roars in victory over your defeat.";
                }
                else if (game.defeatedEnemyType() == "Troll")
                {
                    victoryText =
                        "The Troll towers over you in triumph.";
                }
                else if (game.defeatedEnemyType() == "Dragon")
                {
                    victoryText =
                        "The Dragon claims victory as you fall.";
                }
                else
                {
                    victoryText =
                        std::string(game.defeatedEnemyType()) +
                        " stands victorious over you.";
                }

                drawWrappedFallbackText(
                    victoryText,
                    PovRenderX + 20,
                    PovRenderY + 155,
                    PovRenderWidth - 40,
                    140,
                    24,
                    32);
            }

            return;
        }

        const Combat* combat =
            game.combat();

        if (combat == nullptr)
        {
            return;
        }

        const CombatTarget& target =
            combat->target();

        // ---------------------------------------------------------------------
        // Chest
        // ---------------------------------------------------------------------

        if (target.targetType() == "Chest")
        {
            const Texture2D& texture =
                target.isDefeated()
                ? m_mainScreenAssets.seesDefeatedChest()
                : m_mainScreenAssets.seesChest();

            if (texture.id != 0)
            {
                drawTexture(
                    texture,
                    PovRenderX,
                    PovRenderY,
                    PovRenderWidth,
                    PovRenderHeight);
            }
            else
            {
                drawFallbackText(
                    target.isDefeated()
                    ? "Defeated Chest"
                    : "Chest",
                    PovRenderX,
                    PovRenderY + 210,
                    PovRenderWidth,
                    50,
                    28);
            }

            return;
        }


        std::string_view enemyType =
            target.targetType();

        for (const auto& enemy : game.enemies())
        {
            if (!enemy)
            {
                continue;
            }

            if (enemy.get() == &target)
            {
                enemyType =
                    enemy->type();
                break;
            }
        }

        const CombatPresentation& presentation =
            game.combatPresentation();

        CombatPovFallbackState fallbackState =
            CombatPovFallbackState::Idle;

        const Texture2D* texture =
            nullptr;

        // ---------------------------------------------------------------------
        // Death
        // ---------------------------------------------------------------------

        if (target.isDefeated())
        {
            fallbackState =
                CombatPovFallbackState::Death;

            texture =
                &m_mainScreenAssets.enemyDeath(
                    enemyType);
        }
        else
        {

            // -----------------------------------------------------------------
            // Idle state
            // -----------------------------------------------------------------

            texture =
                &m_mainScreenAssets.enemyIdle(
                    enemyType);

            // -------------------------------------------------------------
            // Hit / Attack state
            // -------------------------------------------------------------

            if (presentation.phase() ==
                CombatPresentation::Phase::Result)
            {
                const AttackResult* result =
                    presentation.currentAttack();

                if (result != nullptr &&
                    result->damage > 0)
                {
                    const auto rollType =
                        presentation.rollType();

                    const bool damageStep =
                        rollType ==
                        CombatPresentation::RollType::Defense
                        ||
                        (rollType ==
                            CombatPresentation::RollType::Wound &&
                            result->defenseRoll == 0);

                    if (damageStep)
                    {
                        if (game.combatPresentationPlayerIsAttacking())
                        {
                            fallbackState =
                                CombatPovFallbackState::Hurt;

                            texture =
                                &m_mainScreenAssets.enemyHurt(
                                    enemyType);
                        }
                        else
                        {
                            fallbackState =
                                CombatPovFallbackState::Attack;

                            texture =
                                &m_mainScreenAssets.enemyAttack(
                                    enemyType);
                        }
                    }
                }
            }
        }

        if (texture != nullptr &&
            texture->id != 0)
        {
            drawTexture(
                *texture,
                PovRenderX,
                PovRenderY,
                PovRenderWidth,
                PovRenderHeight);

            return;
        }


        const std::string fallbackText =
            combatEnemyFallbackText(
                enemyType,
                fallbackState);

        drawWrappedFallbackText(
            fallbackText,
            PovRenderX + 20,
            PovRenderY + 155,
            PovRenderWidth - 40,
            140,
            24,
            32);
    }

    // =========================================================================
    // Dice Roll
    // =========================================================================
    void Renderer::drawDiceRoll(const Game& game) const
    {
        const CombatPresentation& presentation = game.combatPresentation();

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

        if (presentation.phase() ==
            CombatPresentation::Phase::Rolling)
        {
            if (m_mainScreenAssets.diceRolling().id != 0)
            {
                drawTexture(
                    m_mainScreenAssets.diceRolling(),
                    dieX,
                    dieY,
                    DieSize,
                    DieSize);
            }
            else
            {
                const int dieValue =
                    static_cast<int>(
                        presentation.phaseElapsed() * 10.0f) % 6 + 1;

                drawFallbackDie(
                    dieValue,
                    dieX,
                    dieY,
                    DieSize);
            }
            return;
        }

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
                dieValue = result->precisionRoll;
                break;

            case CombatPresentation::RollType::Wound:
                dieValue = result->woundRoll;
                break;

            case CombatPresentation::RollType::Defense:
                dieValue = result->defenseRoll;
                break;

            case CombatPresentation::RollType::None:
            default:
                return;
            }

            const Texture2D& resultTexture =
                m_mainScreenAssets.diceResult(dieValue);

            if (resultTexture.id != 0)
            {
                drawTexture(
                    resultTexture,
                    dieX,
                    dieY,
                    DieSize,
                    DieSize);
            }
            else
            {
                drawFallbackDie(
                    dieValue,
                    dieX,
                    dieY,
                    DieSize);
            }
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

    void Renderer::drawCombatInfo(const Game& game) const
    {
        if (game.defeatPromptActive())
        {
            return;
        }

        const CombatPresentation& presentation = game.combatPresentation();

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
        const std::string_view message =
            !game.combatInfoMessage().empty()
            ? std::string_view(game.combatInfoMessage())
            : std::string_view(presentation.infoMessage());
        drawWrappedFallbackText(
            message,
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

    void Renderer::resetMapCamera() noexcept
    {
        m_mapCameraX = 0;
        m_mapCameraY = 0;
        m_mapCameraInitialized = false;
        m_lastCameraMapWidth = 0;
        m_lastCameraMapHeight = 0;
    }

    void Renderer::updateMapCamera(const Game& game) const
    {
        const Map& map =
            game.map();

        const Player& player =
            game.player();

        const int mapWidth =
            static_cast<int>(map.width());

        const int mapHeight =
            static_cast<int>(map.height());

        const int visibleTilesX =
            VisibleMapWidth /
            TemporaryMapTileSize;

        const int visibleTilesY =
            VisibleMapHeight /
            TemporaryMapTileSize;

        const int maxCameraX =
            std::max(
                0,
                mapWidth - visibleTilesX);

        const int maxCameraY =
            std::max(
                0,
                mapHeight - visibleTilesY);

        if (!m_mapCameraInitialized ||
            m_lastCameraMapWidth != mapWidth ||
            m_lastCameraMapHeight != mapHeight)
        {
            m_mapCameraX =
                std::clamp(
                    player.x() -
                    (visibleTilesX / 2),
                    0,
                    maxCameraX);

            m_mapCameraY =
                std::clamp(
                    player.y() -
                    (visibleTilesY / 2),
                    0,
                    maxCameraY);

            m_mapCameraInitialized = true;
            m_lastCameraMapWidth = mapWidth;
            m_lastCameraMapHeight = mapHeight;
        }

        const int playerCameraX =
            player.x() -
            m_mapCameraX;

        if (playerCameraX < CameraDeadZoneLeft)
        {
            m_mapCameraX =
                player.x() -
                CameraDeadZoneLeft;
        }
        else if (playerCameraX > CameraDeadZoneRight)
        {
            m_mapCameraX =
                player.x() -
                CameraDeadZoneRight;
        }

        const int playerCameraY =
            player.y() -
            m_mapCameraY;

        if (playerCameraY < CameraDeadZoneBottom)
        {
            m_mapCameraY =
                player.y() -
                CameraDeadZoneBottom;
        }
        else if (playerCameraY > CameraDeadZoneTop)
        {
            m_mapCameraY =
                player.y() -
                CameraDeadZoneTop;
        }

        m_mapCameraX =
            std::clamp(
                m_mapCameraX,
                0,
                maxCameraX);

        m_mapCameraY =
            std::clamp(
                m_mapCameraY,
                0,
                maxCameraY);
    }

    void Renderer::drawMap(const Game& game) const
    {
        const Map& map =
            game.map();

        updateMapCamera(game);

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
        // Fixed World Viewport
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

        const int mapWidth =
            static_cast<int>(map.width());

        const int mapHeight =
            static_cast<int>(map.height());

        const int visibleTilesX =
            VisibleMapWidth /
            TemporaryMapTileSize;

        const int visibleTilesY =
            VisibleMapHeight /
            TemporaryMapTileSize;

        const int mapPixelWidth =
            mapWidth *
            TemporaryMapTileSize;

        const int mapPixelHeight =
            mapHeight *
            TemporaryMapTileSize;

        const int mapOffsetX =
            std::max(
                0,
                (VisibleMapWidth - mapPixelWidth) / 2);

        const int mapOffsetY =
            std::max(
                0,
                (VisibleMapHeight - mapPixelHeight) / 2);


        const int firstWorldX =
            m_mapCameraX;

        const int firstWorldY =
            m_mapCameraY;

        const int lastWorldX =
            std::min(
                mapWidth - 1,
                m_mapCameraX + visibleTilesX - 1);

        const int lastWorldY =
            std::min(
                mapHeight - 1,
                m_mapCameraY + visibleTilesY - 1);

        BeginScissorMode(
            static_cast<int>(visibleMap.x),
            static_cast<int>(visibleMap.y),
            VisibleMapWidth,
            VisibleMapHeight);

        // ---------------------------------------------------------------------
        // Static Map Tiles
        // ---------------------------------------------------------------------

        for (int worldY = firstWorldY;
            worldY <= lastWorldY;
            ++worldY)
        {
            for (int worldX = firstWorldX;
                worldX <= lastWorldX;
                ++worldX)
            {
                const char tile =
                    map.tileAt(
                        worldX,
                        worldY);

                const int localX =
                    worldX -
                    m_mapCameraX;

                const int localY =
                    worldY -
                    m_mapCameraY;

                const int tileX =
                    visibleMapX +
                    mapOffsetX +
                    localX *
                    TemporaryMapTileSize;

                const int tileY =
                    visibleMapY -
                    mapOffsetY +
                    VisibleMapHeight -
                    TemporaryMapTileSize -
                    localY *
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
                }
                else if (tile == '#')
                {
                    DrawRectangleRec(
                        tileRectangle,
                        DARKGRAY);
                }
                else if (tile == '@')
                {
                    DrawRectangleRec(
                        tileRectangle,
                        PURPLE);

                    DrawRectangleLinesEx(
                        tileRectangle,
                        2.0f,
                        BLACK);

                    drawFallbackText(
                        "@",
                        tileX,
                        tileY,
                        TemporaryMapTileSize,
                        TemporaryMapTileSize,
                        18);
                }
                else
                {
                    DrawRectangleRec(
                        tileRectangle,
                        GRAY);
                }

                if (game.isZoo())
                {
                    const bool zooStation =
                        tile == 'H' ||
                        tile == 'W' ||
                        tile == 'w' ||
                        tile == 'A' ||
                        tile == 'a' ||
                        tile == 'X' ||
                        tile == 'x' ||
                        tile == 'R';

                    if (zooStation)
                    {
                        DrawRectangleRec(
                            tileRectangle,
                            GOLD);

                        drawFallbackText(
                            std::string(1, tile),
                            tileX,
                            tileY,
                            TemporaryMapTileSize,
                            TemporaryMapTileSize,
                            11);
                    }
                }
            }
        }

        const auto isVisible =
            [&](int worldX, int worldY)
            {
                return worldX >= firstWorldX &&
                    worldX <= lastWorldX &&
                    worldY >= firstWorldY &&
                    worldY <= lastWorldY;
            };

        const auto worldToViewX =
            [&](int worldX)
            {
                return visibleMapX +
                    mapOffsetX +
                    (worldX - m_mapCameraX) *
                    TemporaryMapTileSize;
            };

        const auto worldToViewY =
            [&](int worldY)
            {
                return visibleMapY -
                    mapOffsetY +
                    VisibleMapHeight -
                    TemporaryMapTileSize -
                    (worldY - m_mapCameraY) *
                    TemporaryMapTileSize;
            };

        // ---------------------------------------------------------------------
        // Player
        // ---------------------------------------------------------------------

        const Player& player =
            game.player();

        if (isVisible(player.x(), player.y()))
        {
            const int playerX =
                worldToViewX(player.x());

            const int playerY =
                worldToViewY(player.y());

            const Texture2D& playerTexture =
                m_mainScreenAssets.playerTile();

            if (playerTexture.id != 0)
            {
                float rotation = 0.0f;

                switch (game.facingDirection())
                {
                case FacingDirection::North:
                    rotation = 0.0f;
                    break;
                case FacingDirection::East:
                    rotation = 90.0f;
                    break;
                case FacingDirection::South:
                    rotation = 180.0f;
                    break;
                case FacingDirection::West:
                    rotation = 270.0f;
                    break;
                }

                drawTextureRotated(
                    playerTexture,
                    playerX,
                    playerY,
                    TemporaryMapTileSize,
                    TemporaryMapTileSize,
                    rotation);
            }
            else
            {
                const Rectangle playerRectangle =
                    toRaylibRectangle(
                        playerX,
                        playerY,
                        TemporaryMapTileSize,
                        TemporaryMapTileSize);

                DrawRectangleRec(
                    playerRectangle,
                    BLUE);

                const float centerX =
                    playerRectangle.x +
                    (TemporaryMapTileSize / 2.0f);

                const float centerY =
                    playerRectangle.y +
                    (TemporaryMapTileSize / 2.0f);

                const float halfSize =
                    TemporaryMapTileSize * 0.35f;

                Vector2 tip{ centerX, centerY - halfSize };
                Vector2 left{ centerX - halfSize, centerY + halfSize };
                Vector2 right{ centerX + halfSize, centerY + halfSize };

                switch (game.facingDirection())
                {
                case FacingDirection::North:
                    break;
                case FacingDirection::East:
                    tip = Vector2{ centerX + halfSize, centerY };
                    left = Vector2{ centerX - halfSize, centerY - halfSize };
                    right = Vector2{ centerX - halfSize, centerY + halfSize };
                    break;
                case FacingDirection::South:
                    tip = Vector2{ centerX, centerY + halfSize };
                    left = Vector2{ centerX + halfSize, centerY - halfSize };
                    right = Vector2{ centerX - halfSize, centerY - halfSize };
                    break;
                case FacingDirection::West:
                    tip = Vector2{ centerX - halfSize, centerY };
                    left = Vector2{ centerX + halfSize, centerY + halfSize };
                    right = Vector2{ centerX + halfSize, centerY - halfSize };
                    break;
                }

                DrawTriangle(tip, left, right, WHITE);
            }
        }

        // ---------------------------------------------------------------------
        // Enemies
        // ---------------------------------------------------------------------

        const auto& enemies =
            game.enemies();

        for (const auto& enemy : enemies)
        {
            if (!enemy ||
                !isVisible(enemy->x(), enemy->y()))
            {
                continue;
            }

            const int enemyX =
                worldToViewX(enemy->x());

            const int enemyY =
                worldToViewY(enemy->y());

            const std::string_view type =
                enemy->type();

            const Texture2D* enemyTexture =
                &m_mainScreenAssets.goblinTile();

            const char* identifier =
                "G";

            if (type == "Skeleton")
            {
                enemyTexture =
                    enemy->isDefeated()
                    ? &m_mainScreenAssets.skeletonDefeatedTile()
                    : &m_mainScreenAssets.skeletonTile();

                identifier = "S";
            }
            else if (type == "Orc")
            {
                enemyTexture =
                    enemy->isDefeated()
                    ? &m_mainScreenAssets.orcDefeatedTile()
                    : &m_mainScreenAssets.orcTile();

                identifier = "O";
            }
            else if (type == "Troll")
            {
                enemyTexture =
                    enemy->isDefeated()
                    ? &m_mainScreenAssets.trollDefeatedTile()
                    : &m_mainScreenAssets.trollTile();

                identifier = "T";
            }
            else if (type == "Dragon")
            {
                enemyTexture =
                    enemy->isDefeated()
                    ? &m_mainScreenAssets.dragonDefeatedTile()
                    : &m_mainScreenAssets.dragonTile();

                identifier = "D";
            }
            else if (type == "Goblin")
            {
                enemyTexture =
                    enemy->isDefeated()
                    ? &m_mainScreenAssets.goblinDefeatedTile()
                    : &m_mainScreenAssets.goblinTile();
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
            if (!chest ||
                !isVisible(chest->x(), chest->y()))
            {
                continue;
            }

            const int chestX =
                worldToViewX(chest->x());

            const int chestY =
                worldToViewY(chest->y());

            const Texture2D& chestTexture =
                chest->isDefeated()
                ? m_mainScreenAssets.chestOpenedTile()
                : m_mainScreenAssets.chestTile();

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

        EndScissorMode();

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