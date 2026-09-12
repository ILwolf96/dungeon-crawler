#include "app/LaunchMenu.h"

#include "config/ConfigData.h"
#include "config/DataRepository.h"

#include <algorithm>
#include <exception>
#include <fstream>
#include <string>
#include <utility>

namespace dungeon //Self Note, the Strings and text is here!!
{
    LaunchMenu::LaunchMenu()
    {
        loadPersistentSettings();
        refreshSelectionInfo("Launch settings loaded.");
    }

    LaunchMenuResult LaunchMenu::handleAction(int action)
    {
        if (action < 1 || action > 4)
        {
            return LaunchMenuResult::None;
        }

        if (m_screen == LaunchScreen::Main)
        {
            return handleMainAction(action);
        }

        return handleSettingsAction(action);
    }

    LaunchMenuResult LaunchMenu::update(float deltaSeconds)
    {
        if (m_messageTimeRemaining <= 0.0f)
        {
            return LaunchMenuResult::None;
        }

        m_messageTimeRemaining =
            std::max(0.0f, m_messageTimeRemaining - deltaSeconds);

        if (m_messageTimeRemaining > 0.0f)
        {
            return LaunchMenuResult::None;
        }

        const LaunchMenuResult result = m_pendingResult;
        m_pendingResult = LaunchMenuResult::None;
        return result;
    }

    LaunchScreen LaunchMenu::screen() const noexcept
    {
        return m_screen;
    }

    bool LaunchMenu::inSettings() const noexcept
    {
        return m_screen == LaunchScreen::Settings;
    }

    LaunchLoadMethod LaunchMenu::loadMethod() const noexcept
    {
        return m_loadMethod;
    }

    LaunchMap LaunchMenu::mapToLoad() const noexcept
    {
        return m_mapToLoad;
    }

    LaunchGraphicPreview LaunchMenu::graphicPreview() const noexcept
    {
        return m_graphicPreview;
    }

    const std::string& LaunchMenu::screenWidth() const noexcept
    {
        return m_screenWidth;
    }

    const std::string& LaunchMenu::screenHeight() const noexcept
    {
        return m_screenHeight;
    }

    const std::string& LaunchMenu::gameResolution() const noexcept
    {
        return m_gameResolution;
    }

    const std::string& LaunchMenu::selectionInfoMessage() const noexcept
    {
        return m_selectionInfoMessage;
    }

    const std::string& LaunchMenu::configFilePath() const noexcept
    {
        return m_configFilePath;
    }

    bool LaunchMenu::configurationReady() const noexcept
    {
        return m_configurationReady;
    }

    LaunchMenuResult LaunchMenu::handleMainAction(int action)
    {
        switch (action)
        {
        case 1:
            if (!m_configurationReady)
            {
                refreshSelectionInfo(
                    "Selected configuration is unavailable. Change Load Method or Map to Load.");
                return LaunchMenuResult::None;
            }

            savePersistentSettings();
            refreshSelectionInfo("Launch Selected, Launching Game");
            m_pendingResult = LaunchMenuResult::Launch;
            m_messageTimeRemaining = 1.0f;
            return LaunchMenuResult::None;

        case 2:
            m_screen = LaunchScreen::Settings;
            refreshSelectionInfo("Settings Selected, Showing Settings");
            return LaunchMenuResult::None;

        case 3:
            savePersistentSettings();
            refreshSelectionInfo("Close Selected, Closing Application");
            m_pendingResult = LaunchMenuResult::Close;
            m_messageTimeRemaining = 1.0f;
            return LaunchMenuResult::None;

        default:
            return LaunchMenuResult::None;
        }
    }

    LaunchMenuResult LaunchMenu::handleSettingsAction(int action)
    {
        switch (action)
        {
        case 1:
            m_loadMethod =
                m_loadMethod == LaunchLoadMethod::Ini
                ? LaunchLoadMethod::Json
                : LaunchLoadMethod::Ini;

            refreshConfigurationDetails();
            savePersistentSettings();
            refreshSelectionInfo("Load Method Changed");
            break;

        case 2:
            m_mapToLoad =
                m_mapToLoad == LaunchMap::Dungeon
                ? LaunchMap::Zoo
                : LaunchMap::Dungeon;

            refreshConfigurationDetails();
            savePersistentSettings();
            refreshSelectionInfo("Loaded Map Has Changed");
            break;

        case 3:
            m_graphicPreview =
                m_graphicPreview == LaunchGraphicPreview::Png
                ? LaunchGraphicPreview::Raylib
                : LaunchGraphicPreview::Png;

            savePersistentSettings();
            refreshSelectionInfo("Graphic Preview has Changed");
            break;

        case 4:
            m_screen = LaunchScreen::Main;
            refreshSelectionInfo("Closing Settings, returning to Launch Menu");
            break;

        default:
            break;
        }

        return LaunchMenuResult::None;
    }

    void LaunchMenu::refreshSelectionInfo(std::string message)
    {
        m_selectionInfoMessage = std::move(message);
        refreshConfigurationDetails();
    }

    void LaunchMenu::refreshConfigurationDetails()
    {
        m_configFilePath = buildConfigFilePath();
        m_configurationReady = false;
        m_screenWidth = "-";
        m_screenHeight = "-";
        m_gameResolution = "-";

        try
        {
            config::DataRepository repository;
            const config::ConfigData data =
                repository.load(m_configFilePath);

            if (!data.hasValue("window", "width") ||
                !data.hasValue("window", "height"))
            {
                return;
            }

            m_screenWidth =
                data.getValue("window", "width");
            m_screenHeight =
                data.getValue("window", "height");
            m_gameResolution =
                m_screenWidth + " x " + m_screenHeight;
            m_configurationReady = true;
        }
        catch (const std::exception&)
        {
            // Unavailable configurations remain visibly unavailable in the menu.
        }
    }

    void LaunchMenu::loadPersistentSettings()
    {
        std::ifstream input{
            std::string(PersistencePath) };

        if (!input)
        {
            return;
        }

        std::string line;
        while (std::getline(input, line))
        {
            const std::size_t equals =
                line.find('=');

            if (equals == std::string::npos)
            {
                continue;
            }

            std::string key = line.substr(0, equals);
            std::string value = line.substr(equals + 1);

            trim(key);
            trim(value);

            if (key == "load_method")
            {
                if (value == "json")
                {
                    m_loadMethod = LaunchLoadMethod::Json;
                }
                else if (value == "ini")
                {
                    m_loadMethod = LaunchLoadMethod::Ini;
                }
            }
            else if (key == "map")
            {
                if (value == "zoo")
                {
                    m_mapToLoad = LaunchMap::Zoo;
                }
                else if (value == "dungeon")
                {
                    m_mapToLoad = LaunchMap::Dungeon;
                }
            }
            else if (key == "graphic_preview")
            {
                if (value == "raylib")
                {
                    m_graphicPreview = LaunchGraphicPreview::Raylib;
                }
                else if (value == "png")
                {
                    m_graphicPreview = LaunchGraphicPreview::Png;
                }
            }
        }
    }

    void LaunchMenu::savePersistentSettings() const
    {
        std::ofstream output{
            std::string(PersistencePath),
            std::ios::trunc };

        if (!output)
        {
            return;
        }

        output
            << "[launch]\n"
            << "load_method="
            << (m_loadMethod == LaunchLoadMethod::Ini ? "ini" : "json")
            << "\n"
            << "map="
            << (m_mapToLoad == LaunchMap::Dungeon ? "dungeon" : "zoo")
            << "\n"
            << "graphic_preview="
            << (m_graphicPreview == LaunchGraphicPreview::Png ? "png" : "raylib")
            << "\n";
    }

    std::string LaunchMenu::buildConfigFilePath() const
    {
        const std::string base =
            m_mapToLoad == LaunchMap::Dungeon
            ? "data/dungeon"
            : "data/zoo";

        return base +
            (m_loadMethod == LaunchLoadMethod::Ini ? ".ini" : ".json");
    }

    const char* LaunchMenu::loadMethodLabel() const noexcept
    {
        return m_loadMethod == LaunchLoadMethod::Ini
            ? "INI"
            : "JSON";
    }

    const char* LaunchMenu::mapLabel() const noexcept
    {
        return m_mapToLoad == LaunchMap::Dungeon
            ? "Dungeon"
            : "Zoo";
    }

    const char* LaunchMenu::graphicPreviewLabel() const noexcept
    {
        return m_graphicPreview == LaunchGraphicPreview::Png
            ? "PNG"
            : "Raylib";
    }

    void LaunchMenu::trim(std::string& value)
    {
        const std::string whitespace = " \t\r\n";

        const std::size_t first =
            value.find_first_not_of(whitespace);

        if (first == std::string::npos)
        {
            value.clear();
            return;
        }

        const std::size_t last =
            value.find_last_not_of(whitespace);

        value = value.substr(
            first,
            last - first + 1);
    }
}