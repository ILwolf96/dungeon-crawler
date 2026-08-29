#include "Game.h"

#include "EnemyFactory.h"
#include "config/ParserFactory.h"

#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace
{
    constexpr std::string_view EnemySectionPrefix = "enemy.";

    bool isEnemySection(std::string_view sectionName)
    {
        return sectionName.starts_with(EnemySectionPrefix);
    }

    int getRequiredInt(
        const config::ConfigData& data,
        std::string_view section,
        std::string_view key)
    {
        const std::string sectionName(section);
        const std::string keyName(key);

        if (!data.hasValue(sectionName, keyName))
        {
            throw std::runtime_error(
                "Missing configuration value: " +
                sectionName + "." +
                keyName);
        }

        try
        {
            return std::stoi(
                data.getValue(sectionName, keyName));
        }
        catch (const std::exception&)
        {
            throw std::runtime_error(
                "Invalid integer value for: " +
                sectionName + "." +
                keyName);
        }
    }

    std::string getRequiredString(
        const config::ConfigData& data,
        std::string_view section,
        std::string_view key)
    {
        const std::string sectionName(section);
        const std::string keyName(key);

        if (!data.hasValue(sectionName, keyName))
        {
            throw std::runtime_error(
                "Missing configuration value: " +
                sectionName + "." +
                keyName);
        }

        const std::string value =
            data.getValue(sectionName, keyName);

        if (value.empty())
        {
            throw std::runtime_error(
                "Configuration value cannot be empty: " +
                sectionName + "." +
                keyName);
        }

        return value;
    }
}

namespace dungeon
{
    Game::Game()
        : m_map(),
        m_player(0, 0)
    {
    }

    void Game::load(std::string_view filePath)
    {
        auto parser = config::makeParser(filePath);
        const config::ConfigData data = parser->parse(filePath);

        const int windowWidth =
            getRequiredInt(data, "window", "width");

        const int windowHeight =
            getRequiredInt(data, "window", "height");

        const std::string title =
            getRequiredString(data, "window", "title");

        if (windowWidth <= 0 || windowHeight <= 0)
        {
            throw std::runtime_error(
                "Window width and height must be greater than zero.");
        }

        const int startX =
            getRequiredInt(data, "player", "start_x");

        const int startY =
            getRequiredInt(data, "player", "start_y");

        const std::size_t height =
            static_cast<std::size_t>(
                getRequiredInt(data, "map", "height"));

        if (height == 0)
        {
            throw std::runtime_error(
                "Map height must be greater than zero.");
        }

        Map::Grid grid;
        grid.reserve(height);

        for (std::size_t row = 0; row < height; ++row)
        {
            const std::string key =
                std::string("row_") +
                (row < 10 ? "0" : "") +
                std::to_string(row);

            grid.push_back(
                getRequiredString(data, "map", key));
        }

        m_map.setGrid(std::move(grid));

        if (!m_map.isWalkable(startX, startY))
        {
            throw std::runtime_error(
                "Player starting position is not walkable.");
        }

        m_player.setPosition(startX, startY);

        m_enemies.clear();

        for (const auto& [sectionName, sectionData] :
            data.sections())
        {
            if (!isEnemySection(sectionName))
            {
                continue;
            }

            const std::string type =
                getRequiredString(data, sectionName, "type");

            const int x =
                getRequiredInt(data, sectionName, "x");

            const int y =
                getRequiredInt(data, sectionName, "y");

            const int maxHp =
                getRequiredInt(data, sectionName, "max_hp");

            const CombatStats stats{
                getRequiredInt(data, sectionName, "attacks"),
                getRequiredInt(data, sectionName, "precision"),
                getRequiredInt(data, sectionName, "strength"),
                getRequiredInt(data, sectionName, "toughness"),
                getRequiredInt(data, sectionName, "defense")
            };

            if (!m_map.isWalkable(x, y))
            {
                throw std::runtime_error(
                    "Enemy '" +
                    sectionName +
                    "' is positioned on a non-walkable tile.");
            }

            if (x == m_player.x() && y == m_player.y())
            {
                throw std::runtime_error(
                    "Enemy '" +
                    sectionName +
                    "' cannot occupy the player's starting position.");
            }

            auto enemy = EnemyFactory::create(
                type,
                maxHp,
                stats,
                x,
                y);

            m_enemies.push_back(std::move(enemy));
        }

        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;
        m_title = title;
    }

    void Game::handleAction(Action action)
    {
        int targetX = m_player.x();
        int targetY = m_player.y();

        switch (action)
        {
        case Action::MoveUp:
            --targetY;
            break;

        case Action::MoveDown:
            ++targetY;
            break;

        case Action::MoveLeft:
            --targetX;
            break;

        case Action::MoveRight:
            ++targetX;
            break;

        case Action::None:
            return;
        }

        if (m_map.isWalkable(targetX, targetY))
        {
            m_player.setPosition(targetX, targetY);
        }
    }

    int Game::windowWidth() const noexcept
    {
        return m_windowWidth;
    }

    int Game::windowHeight() const noexcept
    {
        return m_windowHeight;
    }

    const std::string& Game::title() const noexcept
    {
        return m_title;
    }

    const Map& Game::map() const noexcept
    {
        return m_map;
    }

    const Player& Game::player() const noexcept
    {
        return m_player;
    }

    const std::vector<std::unique_ptr<Enemy>>& Game::enemies() const noexcept
    {
        return m_enemies;
    }
}