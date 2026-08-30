#pragma once

#include <string>

namespace dungeon
{
    class Armor
    {
    public:
        Armor(
            std::string name,
            int tier,
            int defense);

        [[nodiscard]]
        const std::string& name() const noexcept;

        [[nodiscard]]
        int tier() const noexcept;

        [[nodiscard]]
        int defense() const noexcept;

    private:
        std::string m_name;
        int m_tier;
        int m_defense;
    };
}