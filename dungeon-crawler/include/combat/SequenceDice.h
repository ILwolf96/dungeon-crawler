#pragma once

#include "combat/IDice.h"

#include <cstddef>
#include <vector>

namespace dungeon
{
    class SequenceDice final : public IDice
    {
    public:
        explicit SequenceDice(
            std::vector<int> rolls);

        [[nodiscard]]
        int rollD6() override;

        [[nodiscard]]
        bool exhausted() const noexcept;

    private:
        std::vector<int> m_rolls;
        std::size_t m_index{ 0 };
    };
}