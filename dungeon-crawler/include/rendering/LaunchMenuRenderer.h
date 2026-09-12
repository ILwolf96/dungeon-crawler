#pragma once

#include "rendering/LaunchMenuAssets.h"

#include <string_view>

namespace dungeon
{
    class LaunchMenu;

    class LaunchMenuRenderer
    {
    public:
        LaunchMenuRenderer() = default;
        ~LaunchMenuRenderer() = default;

        LaunchMenuRenderer(const LaunchMenuRenderer&) = delete;
        LaunchMenuRenderer& operator=(const LaunchMenuRenderer&) = delete;

        [[nodiscard]] bool loadAssets();
        void unloadAssets() noexcept;

        void draw(const LaunchMenu& launchMenu) const;

    private:
        static constexpr int WindowWidth = 470;
        static constexpr int WindowHeight = 520;

        static constexpr int LaunchTitleX = 0;
        static constexpr int LaunchTitleY = 0;
        static constexpr int LaunchTitleWidth = 470;
        static constexpr int LaunchTitleHeight = 50;

        static constexpr int CurrentSettingsTitleX = 0;
        static constexpr int CurrentSettingsTitleY = 50;
        static constexpr int CurrentSettingsTitleWidth = 230;
        static constexpr int CurrentSettingsTitleHeight = 50;

        static constexpr int PreviewTitleX = 230;
        static constexpr int PreviewTitleY = 50;
        static constexpr int PreviewTitleWidth = 240;
        static constexpr int PreviewTitleHeight = 50;

        static constexpr int CurrentSettingsWindowX = 0;
        static constexpr int CurrentSettingsWindowY = 100;
        static constexpr int CurrentSettingsWindowWidth = 230;
        static constexpr int CurrentSettingsWindowHeight = 360;

        static constexpr int PreviewX = 230;
        static constexpr int PreviewY = 100;
        static constexpr int PreviewWidth = 240;
        static constexpr int PreviewHeight = 210;

        static constexpr int SelectionInfoTitleX = 230;
        static constexpr int SelectionInfoTitleY = 310;
        static constexpr int SelectionInfoTitleWidth = 240;
        static constexpr int SelectionInfoTitleHeight = 50;

        static constexpr int SelectionInfoWindowX = 230;
        static constexpr int SelectionInfoWindowY = 360;
        static constexpr int SelectionInfoWindowWidth = 240;
        static constexpr int SelectionInfoWindowHeight = 100;

        static constexpr int LaunchActionsTitleX = 0;
        static constexpr int LaunchActionsTitleY = 460;
        static constexpr int LaunchActionsTitleWidth = 104;
        static constexpr int LaunchActionsTitleHeight = 60;

        static constexpr int ActionsWindowX = 104;
        static constexpr int ActionsWindowY = 460;
        static constexpr int ActionsWindowWidth = 366;
        static constexpr int ActionsWindowHeight = 60;

        void drawPanel(
            const Texture2D& texture,
            int x,
            int y,
            int width,
            int height,
            std::string_view fallbackTitle) const;

        void drawTexture(
            const Texture2D& texture,
            int x,
            int y,
            int width,
            int height) const;

        void drawFallbackText(
            std::string_view text,
            int x,
            int y,
            int width,
            int height,
            int fontSize) const;

        void drawWrappedFallbackText(
            std::string_view text,
            int x,
            int y,
            int width,
            int height,
            int fontSize,
            int lineSpacing) const;

        void drawFallbackPreview(const LaunchMenu& launchMenu) const;
        void drawFallbackActions(const LaunchMenu& launchMenu) const;

        LaunchMenuAssets m_assets;
    };
}