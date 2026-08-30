#include "RandomDice.h"

namespace dungeon
{
    RandomDice::RandomDice()
        : m_engine(m_randomDevice()),
        m_d6(1, 6)
    {
    }

    int RandomDice::rollD6()
    {
        return m_d6(m_engine);
    }
}