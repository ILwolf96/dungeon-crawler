#pragma once

#include "raylib.h"

#include <string>

namespace dungeon
{
    class LaunchMenuAssets
    {
    public:
        LaunchMenuAssets() = default;
        ~LaunchMenuAssets();

        LaunchMenuAssets(const LaunchMenuAssets&) = delete;
        LaunchMenuAssets& operator=(const LaunchMenuAssets&) = delete;

        [[nodiscard]] bool load();
        void unload() noexcept;
        [[nodiscard]] bool loaded() const noexcept;

        [[nodiscard]] const Texture2D& launchTitle() const noexcept;
        [[nodiscard]] const Texture2D& previewTitle() const noexcept;
        [[nodiscard]] const Texture2D& selectionInfoTitle() const noexcept;
        [[nodiscard]] const Texture2D& currentSettingsTitle() const noexcept;
        [[nodiscard]] const Texture2D& launchActionsTitle() const noexcept;

        [[nodiscard]] const Texture2D& selectionInfoWindowBackground() const noexcept;
        [[nodiscard]] const Texture2D& currentSettingsWindowBackground() const noexcept;
        [[nodiscard]] const Texture2D& launchActions() const noexcept;
        [[nodiscard]] const Texture2D& settingsActions() const noexcept;
        [[nodiscard]] const Texture2D& pngPreview() const noexcept;
        [[nodiscard]] const Texture2D& raylibPreview() const noexcept;

    private:
        static Texture2D emptyTexture() noexcept;
        static Texture2D loadTextureIfPresent(
            const std::string& path,
            bool& loadedSuccessfully);
        static bool isValidTexture(const Texture2D& texture) noexcept;

        Texture2D m_launchTitle{};
        Texture2D m_previewTitle{};
        Texture2D m_selectionInfoTitle{};
        Texture2D m_currentSettingsTitle{};
        Texture2D m_launchActionsTitle{};
        Texture2D m_selectionInfoWindowBackground{};
        Texture2D m_currentSettingsWindowBackground{};
        Texture2D m_launchActions{};
        Texture2D m_settingsActions{};
        Texture2D m_pngPreview{};
        Texture2D m_raylibPreview{};

        bool m_loaded{ false };
    };
}