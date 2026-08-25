#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace dungeon
{
    class Map
    {
    public:
        using Row = std::string;
        using Grid = std::vector<Row>;

        Map() = default;

        [[nodiscard]]
        std::size_t width() const noexcept;

        [[nodiscard]]
        std::size_t height() const noexcept;

        [[nodiscard]]
        bool isInside(int x, int y) const noexcept;

        [[nodiscard]]
        bool isWalkable(int x, int y) const noexcept;

        [[nodiscard]]
        char tileAt(int x, int y) const;

        void setGrid(Grid grid);

        [[nodiscard]]
        const Grid& grid() const noexcept;

    private:
        Grid m_grid;
    };
}