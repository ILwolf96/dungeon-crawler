#pragma once

#include <string>

namespace dungeon
{
    class Weapon
    {
    public:
        Weapon(
            std::string name,
            int tier,
            int strength,
            int damage);

        [[nodiscard]]
        const std::string& name() const noexcept;

        [[nodiscard]]
        int tier() const noexcept;

        [[nodiscard]]
        int strength() const noexcept;

        [[nodiscard]]
        int damage() const noexcept;

    private:
        std::string m_name;
        int m_tier;
        int m_strength;
        int m_damage;
    };
}