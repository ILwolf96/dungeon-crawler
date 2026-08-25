#pragma once

namespace dungeon
{
    class Player
    {
    public:
        Player() = default;

        Player(int x, int y);

        [[nodiscard]]
        int x() const noexcept;

        [[nodiscard]]
        int y() const noexcept;

        void setPosition(int x, int y);

    private:
        int m_x{ 0 };
        int m_y{ 0 };
    };
}