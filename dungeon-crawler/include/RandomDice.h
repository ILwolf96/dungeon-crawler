#pragma once

#include "IDice.h"

#include <random>

namespace dungeon
{
    class RandomDice final : public IDice
    {
    public:
        RandomDice();

        [[nodiscard]]
        int rollD6() override;

    private:
        std::random_device m_randomDevice;
        std::mt19937 m_engine;
        std::uniform_int_distribution<int> m_d6;
    };
}