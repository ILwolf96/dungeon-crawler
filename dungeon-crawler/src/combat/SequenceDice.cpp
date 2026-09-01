#include "combat/SequenceDice.h"

#include <stdexcept>
#include <utility>

namespace dungeon
{
    SequenceDice::SequenceDice(
        std::vector<int> rolls)
        : m_rolls(std::move(rolls))
    {
        for (const int roll : m_rolls)
        {
            if (roll < 1 || roll > 6)
            {
                throw std::invalid_argument(
                    "SequenceDice values must be between 1 and 6.");
            }
        }
    }

    int SequenceDice::rollD6()
    {
        if (m_index >= m_rolls.size())
        {
            throw std::out_of_range(
                "SequenceDice has no rolls remaining.");
        }

        return m_rolls[m_index++];
    }

    bool SequenceDice::exhausted() const noexcept
    {
        return m_index >= m_rolls.size();
    }
}