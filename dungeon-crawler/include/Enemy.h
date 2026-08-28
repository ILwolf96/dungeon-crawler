#pragma once

#include <string_view>

namespace dungeon
{
    class Enemy
    {
    public:
        virtual ~Enemy() = default;

        [[nodiscard]]
        virtual std::string_view type() const noexcept = 0;

        [[nodiscard]]
        int x() const noexcept;

        [[nodiscard]]
        int y() const noexcept;

        [[nodiscard]]
        int currentHp() const noexcept;

        [[nodiscard]]
        int maxHp() const noexcept;

        [[nodiscard]]
        int attack() const noexcept;

        [[nodiscard]]
        int defense() const noexcept;

        void setPosition(int x, int y);

        void takeDamage(int amount);

    protected:
        Enemy(
            int maxHp,
            int attack,
            int defense,
            int x,
            int y);

    private:
        int m_x;
        int m_y;

        int m_currentHp;
        int m_maxHp;

        int m_attack;
        int m_defense;
    };
}