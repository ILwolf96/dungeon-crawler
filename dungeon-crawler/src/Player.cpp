#include "Player.h"

namespace dungeon
{
    Player::Player(int x, int y)
        : m_x(x),
        m_y(y)
    {
    }

    int Player::x() const noexcept
    {
        return m_x;
    }

    int Player::y() const noexcept
    {
        return m_y;
    }

    void Player::setPosition(int x, int y)
    {
        m_x = x;
        m_y = y;
    }
}