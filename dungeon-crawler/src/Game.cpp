#include "Game.h"
#include <string>
#include <utility>
#include "config/ParserFactory.h"

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
            const std::string key = std::string("row_") + (row < 10 ? "0" : "") + std::to_string(row);

            grid.push_back(data.getValue("map", key));
        }

        m_map.setGrid(std::move(grid));
        m_player.setPosition(startX, startY);
    }

    void Game::update()
    {
        // Game-state updates will be implemented here.
    }

    const Map& Game::map() const noexcept
    {
        return m_map;
    }

    const Player& Game::player() const noexcept
    {
        return m_player;
    }
}