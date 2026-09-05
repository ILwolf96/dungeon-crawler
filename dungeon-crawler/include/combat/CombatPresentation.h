#pragma once

#include "combat/CombatResult.h"

#include <cstddef>
#include <string>
#include <vector>

namespace dungeon
{
    class CombatPresentation
    {
    public:
        enum class Phase
        {
            None,
            Information,
            Rolling,
            Result
        };

        enum class RollType
        {
            None,
            Precision,
            Wound,
            Defense
        };

        static constexpr float InformationDuration = 1.5f;
        static constexpr float RollingDuration = 0.75f;
        static constexpr float ResultDuration = 1.5f;

        CombatPresentation() = default;

        void start(
            std::string attackerName,
            std::string defenderName,
            std::string selectedMessage,
            std::vector<AttackResult> attacks);

        // Returns true when the presentation completed during this update.
        bool update(float deltaSeconds);

        [[nodiscard]]
        bool active() const noexcept;

        [[nodiscard]]
        Phase phase() const noexcept;

        [[nodiscard]]
        RollType rollType() const noexcept;

        [[nodiscard]]
        const std::string& attackerName() const noexcept;

        [[nodiscard]]
        const std::string& defenderName() const noexcept;

        [[nodiscard]]
        const std::string& infoMessage() const noexcept;

        [[nodiscard]]
        const AttackResult* currentAttack() const noexcept;

        [[nodiscard]]
        std::size_t currentAttackIndex() const noexcept;

        [[nodiscard]]
        float phaseElapsed() const noexcept;

    private:
        void beginRolling(
            RollType rollType);

        void beginResult(
            RollType rollType);

        void advanceAfterResult();

        bool precisionPassed() const noexcept;
        bool woundPassed() const noexcept;
        bool hasDefenseRoll() const noexcept;

        Phase m_phase{ Phase::None };
        RollType m_rollType{ RollType::None };

        std::string m_infoMessage;
        std::string m_attackerName;
        std::string m_defenderName;

        std::vector<AttackResult> m_attacks;
        std::size_t m_attackIndex{ 0 };

        float m_phaseElapsed{ 0.0f };
    };
}