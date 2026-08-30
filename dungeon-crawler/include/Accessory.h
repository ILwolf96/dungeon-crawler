#pragma once

#include <string>

namespace dungeon
{
    class Accessory
    {
    public:
        Accessory(
            std::string name,
            int tier,
            int attacksBonus,
            int precisionBonus,
            int maxHpBonus);

        [[nodiscard]]
        const std::string& name() const noexcept;

        [[nodiscard]]
        int tier() const noexcept;

        [[nodiscard]]
        int attacksBonus() const noexcept;

        [[nodiscard]]
        int precisionBonus() const noexcept;

        [[nodiscard]]
        int maxHpBonus() const noexcept;

    private:
        std::string m_name;
        int m_tier;
        int m_attacksBonus;
        int m_precisionBonus;
        int m_maxHpBonus;
    };
}