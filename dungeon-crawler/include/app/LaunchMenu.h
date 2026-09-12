#pragma once

#include <string>
#include <string_view>

namespace dungeon
{
    enum class LaunchScreen
    {
        Main,
        Settings
    };

    enum class LaunchLoadMethod
    {
        Ini,
        Json
    };

    enum class LaunchMap
    {
        Dungeon,
        Zoo
    };

    enum class LaunchGraphicPreview
    {
        Png,
        Raylib
    };

    enum class LaunchMenuResult
    {
        None,
        Launch,
        Close
    };

    class LaunchMenu
    {
    public:
        LaunchMenu();

        LaunchMenuResult handleAction(int action);
        LaunchMenuResult update(float deltaSeconds);

        [[nodiscard]] LaunchScreen screen() const noexcept;
        [[nodiscard]] bool inSettings() const noexcept;

        [[nodiscard]] LaunchLoadMethod loadMethod() const noexcept;
        [[nodiscard]] LaunchMap mapToLoad() const noexcept;
        [[nodiscard]] LaunchGraphicPreview graphicPreview() const noexcept;

        [[nodiscard]] const std::string& screenWidth() const noexcept;
        [[nodiscard]] const std::string& screenHeight() const noexcept;
        [[nodiscard]] const std::string& gameResolution() const noexcept;
        [[nodiscard]] const std::string& selectionInfoMessage() const noexcept;
        [[nodiscard]] const std::string& configFilePath() const noexcept;

        [[nodiscard]] bool configurationReady() const noexcept;

        [[nodiscard]] static constexpr int windowWidth() noexcept
        {
            return LaunchWindowWidth;
        }

        [[nodiscard]] static constexpr int windowHeight() noexcept
        {
            return LaunchWindowHeight;
        }

    private:
        static constexpr int LaunchWindowWidth = 470;
        static constexpr int LaunchWindowHeight = 520;
        static constexpr std::string_view PersistencePath = "data/launch_settings.ini";

        LaunchScreen m_screen{ LaunchScreen::Main };
        LaunchLoadMethod m_loadMethod{ LaunchLoadMethod::Ini };
        LaunchMap m_mapToLoad{ LaunchMap::Dungeon };
        LaunchGraphicPreview m_graphicPreview{ LaunchGraphicPreview::Png };

        std::string m_screenWidth{ "-" };
        std::string m_screenHeight{ "-" };
        std::string m_gameResolution{ "-" };
        std::string m_configFilePath;
        std::string m_selectionInfoMessage{"Showing Main Launch Menu"};

        bool m_configurationReady{ false };

        LaunchMenuResult m_pendingResult{LaunchMenuResult::None};

        float m_messageTimeRemaining{ 0.0f };

        LaunchMenuResult handleMainAction(int action);
        LaunchMenuResult handleSettingsAction(int action);

        void refreshSelectionInfo(std::string message);
        void refreshConfigurationDetails();

        void loadPersistentSettings();
        void savePersistentSettings() const;

        [[nodiscard]] std::string buildConfigFilePath() const;

        [[nodiscard]] const char* loadMethodLabel() const noexcept;
        [[nodiscard]] const char* mapLabel() const noexcept;
        [[nodiscard]] const char* graphicPreviewLabel() const noexcept;

        static void trim(std::string& value);
    };
}