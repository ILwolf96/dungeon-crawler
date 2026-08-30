#include "world/Map.h"

#include <stdexcept>

namespace dungeon
{
    std::size_t Map::width() const noexcept
    {
        if (m_grid.empty())
        {
            return 0;
        }

        return m_grid.front().size();
    }

    std::size_t Map::height() const noexcept
    {
        return m_grid.size();
    }

    bool Map::isInside(int x, int y) const noexcept
    {
        if (x < 0 || y < 0)
        {
            return false;
        }

        const auto mapX = static_cast<std::size_t>(x);
        const auto mapY = static_cast<std::size_t>(y);

        return mapY < m_grid.size() &&
            mapX < width();
    }

    bool Map::isWalkable(int x, int y) const noexcept
    {
        if (!isInside(x, y))
        {
            return false;
        }

        return m_grid[static_cast<std::size_t>(y)]
            [static_cast<std::size_t>(x)] != '#';
    }

    char Map::tileAt(int x, int y) const
    {
        if (!isInside(x, y))
        {
            throw std::out_of_range("Map coordinates are outside the map.");
        }

        return m_grid[static_cast<std::size_t>(y)]
            [static_cast<std::size_t>(x)];
    }

    void Map::setGrid(Grid grid)
    {
        if (!grid.empty())
        {
            const auto expectedWidth = grid.front().size();

            for (const auto& row : grid)
            {
                if (row.size() != expectedWidth)
                {
                    throw std::invalid_argument(
                        "Map rows must all have the same width.");
                }
            }
        }

        m_grid = std::move(grid);
    }

    const Map::Grid& Map::grid() const noexcept
    {
        return m_grid;
    }
}