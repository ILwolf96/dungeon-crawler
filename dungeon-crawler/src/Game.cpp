#include "Game.h"

#include "config/ParserFactory.h"

#include "EnemyFactory.h"
#include <stdexcept>
#include <string>
#include <utility>

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
            std::stoi(data.getValue("window", "width"));

        const int windowHeight =
            std::stoi(data.getValue("window", "height"));

        const std::string title =
            data.getValue("window", "title");

        if (windowWidth <= 0 || windowHeight <= 0)
        {
            throw std::runtime_error(
                "Window width and height must be greater than zero.");
        }

        const int startX =
            std::stoi(data.getValue("player", "start_x"));

        const int startY =
            std::stoi(data.getValue("player", "start_y"));

        const std::size_t height =
            static_cast<std::size_t>(
                std::stoul(data.getValue("map", "height")));

        Map::Grid grid;
        grid.reserve(height);

        for (std::size_t row = 0; row < height; ++row)
        {
            const std::string key =
                std::string("row_") +
                (row < 10 ? "0" : "") +
                std::to_string(row);

            grid.push_back(data.getValue("map", key));
        }

        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;
        m_title = title;

        m_map.setGrid(std::move(grid));
        m_player.setPosition(startX, startY);
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