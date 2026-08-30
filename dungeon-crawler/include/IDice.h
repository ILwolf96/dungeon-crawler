#pragma once

namespace dungeon
{
    class IDice
    {
    public:
        virtual ~IDice() = default;

        [[nodiscard]]
        virtual int rollD6() = 0;
    };
}