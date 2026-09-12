#include "rendering/LaunchMenuAssets.h"

#include "raylib.h"

#include <string>

namespace
{
    constexpr const char* AssetPath =
        "assets/ui/launch_menu/";
}

namespace dungeon
{
    LaunchMenuAssets::~LaunchMenuAssets()
    {
        unload();
    }

    bool LaunchMenuAssets::isValidTexture(
        const Texture2D& texture) noexcept
    {
        return texture.id != 0;
    }

    Texture2D LaunchMenuAssets::emptyTexture() noexcept
    {
        return Texture2D{};
    }

    Texture2D LaunchMenuAssets::loadTextureIfPresent(
        const std::string& path,
        bool& loadedSuccessfully)
    {
        if (!FileExists(path.c_str()))
        {
            loadedSuccessfully = false;
            return emptyTexture();
        }

        Texture2D texture = LoadTexture(path.c_str());

        if (!isValidTexture(texture))
        {
            loadedSuccessfully = false;
            return emptyTexture();
        }

        loadedSuccessfully = true;
        return texture;
    }

    bool LaunchMenuAssets::load()
    {
        unload();

        bool allLoaded = true;
        bool loaded = false;

        m_launchTitle = loadTextureIfPresent(
            std::string(AssetPath) + "launch_title.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_previewTitle = loadTextureIfPresent(
            std::string(AssetPath) + "preview_title.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_selectionInfoTitle = loadTextureIfPresent(
            std::string(AssetPath) + "selection_info_title.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_currentSettingsTitle = loadTextureIfPresent(
            std::string(AssetPath) + "current_settings_title.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_launchActionsTitle = loadTextureIfPresent(
            std::string(AssetPath) + "launch_actions_title.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_selectionInfoWindowBackground = loadTextureIfPresent(
            std::string(AssetPath) + "selection_info_window_bg.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_currentSettingsWindowBackground = loadTextureIfPresent(
            std::string(AssetPath) + "current_settings_window_bg.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_launchActions = loadTextureIfPresent(
            std::string(AssetPath) + "launch_actions.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_settingsActions = loadTextureIfPresent(
            std::string(AssetPath) + "settings_actions.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_pngPreview = loadTextureIfPresent(
            std::string(AssetPath) + "png_preview.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_raylibPreview = loadTextureIfPresent(
            std::string(AssetPath) + "raylib_preview.png",
            loaded);
        allLoaded = allLoaded && loaded;

        m_loaded = allLoaded;
        return m_loaded;
    }

    void LaunchMenuAssets::unload() noexcept
    {
        auto unloadTexture =
            [](Texture2D& texture)
            {
                if (texture.id != 0)
                {
                    UnloadTexture(texture);
                    texture = Texture2D{};
                }
            };

        unloadTexture(m_launchTitle);
        unloadTexture(m_previewTitle);
        unloadTexture(m_selectionInfoTitle);
        unloadTexture(m_currentSettingsTitle);
        unloadTexture(m_launchActionsTitle);
        unloadTexture(m_selectionInfoWindowBackground);
        unloadTexture(m_currentSettingsWindowBackground);
        unloadTexture(m_launchActions);
        unloadTexture(m_settingsActions);
        unloadTexture(m_pngPreview);
        unloadTexture(m_raylibPreview);

        m_loaded = false;
    }

    bool LaunchMenuAssets::loaded() const noexcept
    {
        return m_loaded;
    }

    const Texture2D& LaunchMenuAssets::launchTitle() const noexcept
    {
        return m_launchTitle;
    }

    const Texture2D& LaunchMenuAssets::previewTitle() const noexcept
    {
        return m_previewTitle;
    }

    const Texture2D& LaunchMenuAssets::selectionInfoTitle() const noexcept
    {
        return m_selectionInfoTitle;
    }

    const Texture2D& LaunchMenuAssets::currentSettingsTitle() const noexcept
    {
        return m_currentSettingsTitle;
    }

    const Texture2D& LaunchMenuAssets::launchActionsTitle() const noexcept
    {
        return m_launchActionsTitle;
    }

    const Texture2D& LaunchMenuAssets::selectionInfoWindowBackground() const noexcept
    {
        return m_selectionInfoWindowBackground;
    }

    const Texture2D& LaunchMenuAssets::currentSettingsWindowBackground() const noexcept
    {
        return m_currentSettingsWindowBackground;
    }

    const Texture2D& LaunchMenuAssets::launchActions() const noexcept
    {
        return m_launchActions;
    }

    const Texture2D& LaunchMenuAssets::settingsActions() const noexcept
    {
        return m_settingsActions;
    }

    const Texture2D& LaunchMenuAssets::pngPreview() const noexcept
    {
        return m_pngPreview;
    }

    const Texture2D& LaunchMenuAssets::raylibPreview() const noexcept
    {
        return m_raylibPreview;
    }
}