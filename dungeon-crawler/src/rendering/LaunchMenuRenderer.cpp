#include "rendering/LaunchMenuRenderer.h"

#include "app/LaunchMenu.h"

#include "raylib.h"

#include <string>
#include <string_view>
#include <vector>

namespace dungeon
{
    namespace
    {
        Color panelBackground()
        {
            return BLACK;
        }

        Color panelBorder()
        {
            return LIGHTGRAY;
        }
    }

    bool LaunchMenuRenderer::loadAssets()
    {
        return m_assets.load();
    }

    void LaunchMenuRenderer::unloadAssets() noexcept
    {
        m_assets.unload();
    }

    void LaunchMenuRenderer::draw(
        const LaunchMenu& launchMenu) const
    {
        ClearBackground(BLACK);

        const bool usePngAssets = launchMenu.graphicPreview() == LaunchGraphicPreview::Png;

        // ---------------------------------------------------------------------
        // Launch Title
        // ---------------------------------------------------------------------

        drawPanel(
            m_assets.launchTitle(),
            LaunchTitleX,
            LaunchTitleY,
            LaunchTitleWidth,
            LaunchTitleHeight,
            "CPP DUNGEON CRAWLER LAUNCH MENU",
            usePngAssets);

        // ---------------------------------------------------------------------
        // Current Settings Title
        // ---------------------------------------------------------------------

        drawPanel(
            m_assets.currentSettingsTitle(),
            CurrentSettingsTitleX,
            CurrentSettingsTitleY,
            CurrentSettingsTitleWidth,
            CurrentSettingsTitleHeight,
            "CURRENT SETTINGS",
            usePngAssets);

        // ---------------------------------------------------------------------
        // Preview Title
        // ---------------------------------------------------------------------

        drawPanel(
            m_assets.previewTitle(),
            PreviewTitleX,
            PreviewTitleY,
            PreviewTitleWidth,
            PreviewTitleHeight,
            "SCREEN PREVIEW",
            usePngAssets);

        // ---------------------------------------------------------------------
        // Current Settings Window
        // ---------------------------------------------------------------------

        drawPanel(
            m_assets.currentSettingsWindowBackground(),
            CurrentSettingsWindowX,
            CurrentSettingsWindowY,
            CurrentSettingsWindowWidth,
            CurrentSettingsWindowHeight,
            "",
            usePngAssets);

        // ---------------------------------------------------------------------
        // Selection Info Window
        // ---------------------------------------------------------------------

        drawPanel(
            m_assets.selectionInfoWindowBackground(),
            SelectionInfoWindowX,
            SelectionInfoWindowY,
            SelectionInfoWindowWidth,
            SelectionInfoWindowHeight,
            "",
            usePngAssets);

        // ---------------------------------------------------------------------
        // Selection Info Title
        // ---------------------------------------------------------------------

        drawPanel(
            m_assets.selectionInfoTitle(),
            SelectionInfoTitleX,
            SelectionInfoTitleY,
            SelectionInfoTitleWidth,
            SelectionInfoTitleHeight,
            "SELECTION INFO",
            usePngAssets);

        // ---------------------------------------------------------------------
        // Launch Actions Title
        // ---------------------------------------------------------------------

        drawPanel(
            m_assets.launchActionsTitle(),
            LaunchActionsTitleX,
            LaunchActionsTitleY,
            LaunchActionsTitleWidth,
            LaunchActionsTitleHeight,
            "ACTIONS",
            usePngAssets);

        // ---------------------------------------------------------------------
        // Launch / Settings Actions
        // ---------------------------------------------------------------------

        const Texture2D& actionsTexture =
            launchMenu.inSettings()
            ? m_assets.settingsActions()
            : m_assets.launchActions();

        drawPanel(
            actionsTexture,
            ActionsWindowX,
            ActionsWindowY,
            ActionsWindowWidth,
            ActionsWindowHeight,
            "",
            usePngAssets);

        if (!usePngAssets || actionsTexture.id == 0)
        {
            drawFallbackActions(
                launchMenu);
        }

        // ---------------------------------------------------------------------
        // Current Settings Text
        // ---------------------------------------------------------------------

        const std::string currentSettings =
            "Screen Height: " +
            launchMenu.screenHeight() +
            "\nScreen Width: " +
            launchMenu.screenWidth() +
            "\nGame Resolution: " +
            launchMenu.gameResolution() +
            "\n\nLoad Method: " +
            std::string(
                launchMenu.loadMethod() ==
                LaunchLoadMethod::Ini
                ? "INI"
                : "JSON") +
            "\nMap Loaded: " +
            std::string(
                launchMenu.mapToLoad() ==
                LaunchMap::Dungeon
                ? "Dungeon"
                : "Zoo") +
            "\nGraphic Preview: " +
            std::string(
                launchMenu.graphicPreview() ==
                LaunchGraphicPreview::Png
                ? "PNG"
                : "Raylib");

        drawWrappedFallbackText(
            currentSettings,
            CurrentSettingsWindowX + 10,
            CurrentSettingsWindowY + 10,
            CurrentSettingsWindowWidth - 20,
            CurrentSettingsWindowHeight - 20,
            14,
            22);

        // ---------------------------------------------------------------------
        // Selection Info Text
        // ---------------------------------------------------------------------

        drawWrappedFallbackText(
            launchMenu.selectionInfoMessage(),
            SelectionInfoWindowX + 10,
            SelectionInfoWindowY + 8,
            SelectionInfoWindowWidth - 20,
            SelectionInfoWindowHeight - 16,
            14,
            20);

        // ---------------------------------------------------------------------
        // Preview
        //
        // PNG mode:
        //     use png_preview.png when and if it exists.
        //
        // Raylib mode:
        //     won't use a PNG preview asset.
        //     and will Use the raylib fallback preview ONLY.
        // ---------------------------------------------------------------------

        if (usePngAssets)
        {
            if (m_assets.pngPreview().id != 0)
            {
                drawTexture(
                    m_assets.pngPreview(),
                    PreviewX,
                    PreviewY,
                    PreviewWidth,
                    PreviewHeight);
            }
            else
            {
                drawFallbackPreview(launchMenu);
            }
        }
        else
        {
            if (m_assets.raylibPreview().id != 0)
            {
                drawTexture(
                    m_assets.raylibPreview(),
                    PreviewX,
                    PreviewY,
                    PreviewWidth,
                    PreviewHeight);
            }
            else
            {
                drawFallbackPreview(launchMenu);
            }
        }
    }

    void LaunchMenuRenderer::drawPanel(
        const Texture2D& texture,
        int x,
        int y,
        int width,
        int height,
        std::string_view fallbackTitle,
        bool usePngAsset) const
    {
        if (usePngAsset &&
            texture.id != 0)
        {
            drawTexture(
                texture,
                x,
                y,
                width,
                height);

            return;
        }

        DrawRectangle(
            x,
            y,
            width,
            height,
            panelBackground());

        DrawRectangleLines(
            x,
            y,
            width,
            height,
            panelBorder());

        if (!fallbackTitle.empty())
        {
            drawFallbackText(
                fallbackTitle,
                x,
                y,
                width,
                height,
                17);
        }
    }

    void LaunchMenuRenderer::drawTexture(
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

        const Rectangle destination{
            static_cast<float>(x),
            static_cast<float>(y),
            static_cast<float>(width),
            static_cast<float>(height)
        };

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
            Vector2{
                0.0f,
                0.0f
            },
            0.0f,
            WHITE);
    }

    void LaunchMenuRenderer::drawFallbackText(
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

        const int textX =
            x +
            (width - textWidth) / 2;

        const int textY =
            y +
            (height - fontSize) / 2;

        DrawText(
            value.c_str(),
            textX,
            textY,
            fontSize,
            RAYWHITE);
    }

    void LaunchMenuRenderer::drawWrappedFallbackText(
        std::string_view text,
        int x,
        int y,
        int width,
        int height,
        int fontSize,
        int lineSpacing) const
    {
        const std::string value(text);

        if (value.empty())
        {
            return;
        }

        constexpr int HorizontalPadding = 4;
        const int maximumWidth =
            width -
            (HorizontalPadding * 2);

        std::vector<std::string> lines;
        std::string currentLine;

        auto pushCurrentLine =
            [&]()
            {
                if (!currentLine.empty())
                {
                    lines.push_back(
                        currentLine);

                    currentLine.clear();
                }
            };

        std::size_t start = 0;
        while (start <= value.size())
        {
            const std::size_t newline =
                value.find(
                    '\n',
                    start);

            const std::size_t sectionEnd =
                newline == std::string::npos
                ? value.size()
                : newline;

            std::size_t wordStart =
                start;

            while (wordStart < sectionEnd)
            {
                std::size_t wordEnd =
                    value.find(
                        ' ',
                        wordStart);

                if (wordEnd == std::string::npos ||
                    wordEnd > sectionEnd)
                {
                    wordEnd =
                        sectionEnd;
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
                        fontSize) <= maximumWidth)
                    {
                        currentLine =
                            candidate;
                    }
                    else
                    {
                        pushCurrentLine();

                        currentLine =
                            word;
                    }
                }

                if (wordEnd >= sectionEnd)
                {
                    break;
                }

                wordStart =
                    wordEnd + 1;
            }

            pushCurrentLine();

            if (newline == std::string::npos)
            {
                break;
            }

            start =
                newline + 1;
        }

        if (lines.empty())
        {
            return;
        }

        const int totalHeight =
            fontSize +
            static_cast<int>(
                lines.size() - 1) *
            lineSpacing;

        int currentY =
            y +
            (height - totalHeight) / 2;

        for (const std::string& line : lines)
        {
            const int lineWidth =
                MeasureText(
                    line.c_str(),
                    fontSize);

            const int lineX =
                x +
                (width - lineWidth) / 2;

            DrawText(
                line.c_str(),
                lineX,
                currentY,
                fontSize,
                RAYWHITE);

            currentY +=
                lineSpacing;
        }
    }

    void LaunchMenuRenderer::drawFallbackActions(
        const LaunchMenu& launchMenu) const
    {
        constexpr int FontSize = 14;

        if (launchMenu.inSettings())
        {
            drawFallbackText(
                "1 - Load Method",
                ActionsWindowX,
                ActionsWindowY + 2,
                180,
                26,
                FontSize);

            drawFallbackText(
                "2 - Change Map",
                ActionsWindowX + 180,
                ActionsWindowY + 2,
                186,
                26,
                FontSize);

            drawFallbackText(
                "3 - Graphic Preview",
                ActionsWindowX,
                ActionsWindowY + 30,
                210,
                26,
                FontSize);

            drawFallbackText(
                "4 - Back",
                ActionsWindowX + 210,
                ActionsWindowY + 30,
                156,
                26,
                FontSize);

            return;
        }

        drawFallbackText(
            "1 - Launch",
            ActionsWindowX,
            ActionsWindowY + 16,
            122,
            28,
            FontSize);

        drawFallbackText(
            "2 - Settings",
            ActionsWindowX + 122,
            ActionsWindowY + 16,
            122,
            28,
            FontSize);

        drawFallbackText(
            "3 - Close",
            ActionsWindowX + 244,
            ActionsWindowY + 16,
            122,
            28,
            FontSize);
    }

    void LaunchMenuRenderer::drawFallbackPreview(
        const LaunchMenu& launchMenu) const
    {
        DrawRectangle(
            PreviewX,
            PreviewY,
            PreviewWidth,
            PreviewHeight,
            DARKGRAY);

        DrawRectangleLines(
            PreviewX,
            PreviewY,
            PreviewWidth,
            PreviewHeight,
            RAYWHITE);

        if (launchMenu.graphicPreview() ==
            LaunchGraphicPreview::Png)
        {
            drawFallbackText(
                "PNG PREVIEW",
                PreviewX,
                PreviewY + 70,
                PreviewWidth,
                40,
                24);

            drawFallbackText(
                "PNG assets",
                PreviewX,
                PreviewY + 120,
                PreviewWidth,
                30,
                18);

            drawFallbackText(
                "with Raylib fallback",
                PreviewX,
                PreviewY + 155,
                PreviewWidth,
                30,
                15);

            return;
        }

        constexpr int cellSize = 42;
        constexpr int gridWidth = 5;
        constexpr int gridHeight = 3;

        constexpr int gridX =
            PreviewX +
            (PreviewWidth -
                gridWidth * cellSize) / 2;

        constexpr int gridY =
            PreviewY + 42;

        for (int row = 0;
            row < gridHeight;
            ++row)
        {
            for (int column = 0;
                column < gridWidth;
                ++column)
            {
                const int x =
                    gridX +
                    column * cellSize;

                const int y =
                    gridY +
                    row * cellSize;

                const Color cellColor =
                    (row == 1 && column == 2)
                    ? MAROON
                    : (
                        row == 0 ||
                        row == gridHeight - 1 ||
                        column == 0 ||
                        column == gridWidth - 1
                        ? LIGHTGRAY
                        : DARKGRAY
                        );

                DrawRectangle(
                    x + 1,
                    y + 1,
                    cellSize - 2,
                    cellSize - 2,
                    cellColor);
            }
        }

        drawFallbackText(
            "RAYLIB FALLBACK",
            PreviewX,
            PreviewY + 175,
            PreviewWidth,
            30,
            18);
    }
}