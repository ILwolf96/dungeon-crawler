#include "Game.h"

namespace dungeon
{
    Game::Game()
        : m_map(),
        m_player(1, 1)
    {
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