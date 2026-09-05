#include "combat/CombatPresentation.h"

#include <algorithm>
#include <string>
#include <utility>

namespace dungeon
{
    void CombatPresentation::start(
        std::string attackerName,
        std::string defenderName,
        std::string selectedMessage,
        std::vector<AttackResult> attacks)
    {
        m_phase = Phase::Information;
        m_rollType = RollType::None;

        m_attackerName = std::move(attackerName);
        m_defenderName = std::move(defenderName);

        m_infoMessage = std::move(selectedMessage);

        m_attacks = std::move(attacks);
        m_attackIndex = 0;

        m_phaseElapsed = 0.0f;

        if (m_attacks.empty())
        {
            m_phase = Phase::None;
            m_infoMessage.clear();
            m_attackerName.clear();
            m_defenderName.clear();
        }
    }

    bool CombatPresentation::update(
        float deltaSeconds)
    {
        if (!active())
        {
            return false;
        }

        if (deltaSeconds < 0.0f)
        {
            deltaSeconds = 0.0f;
        }

        m_phaseElapsed += deltaSeconds;

        switch (m_phase)
        {
        case Phase::Information:
            if (m_phaseElapsed >= InformationDuration)
            {
                beginRolling(RollType::Precision);
            }
            return false;

        case Phase::Rolling:
            if (m_phaseElapsed >= RollingDuration)
            {
                beginResult(m_rollType);
            }
            return false;

        case Phase::Result:
            if (m_phaseElapsed >= ResultDuration)
            {
                advanceAfterResult();
            }

            return !active();

        case Phase::None:
        default:
            return false;
        }
    }

    bool CombatPresentation::active() const noexcept
    {
        return m_phase != Phase::None;
    }

    CombatPresentation::Phase
        CombatPresentation::phase() const noexcept
    {
        return m_phase;
    }

    CombatPresentation::RollType
        CombatPresentation::rollType() const noexcept
    {
        return m_rollType;
    }

    const std::string&
        CombatPresentation::attackerName() const noexcept
    {
        return m_attackerName;
    }

    const std::string&
        CombatPresentation::defenderName() const noexcept
    {
        return m_defenderName;
    }

    const std::string&
        CombatPresentation::infoMessage() const noexcept
    {
        return m_infoMessage;
    }

    const AttackResult*
        CombatPresentation::currentAttack() const noexcept
    {
        if (m_attackIndex >= m_attacks.size())
        {
            return nullptr;
        }

        return &m_attacks[m_attackIndex];
    }

    std::size_t
        CombatPresentation::currentAttackIndex() const noexcept
    {
        return m_attackIndex;
    }

    float CombatPresentation::phaseElapsed() const noexcept
    {
        return m_phaseElapsed;
    }

    void CombatPresentation::beginRolling(
        RollType rollType)
    {
        m_phase = Phase::Rolling;
        m_rollType = rollType;
        m_phaseElapsed = 0.0f;

        switch (rollType)
        {
        case RollType::Precision:
            m_infoMessage =
                m_attackerName +
                "\nRolling Precision Roll\nagainst " +
                m_defenderName;
            break;

        case RollType::Wound:
            m_infoMessage =
                m_attackerName +
                "\nRolling Wound Roll\nagainst " +
                m_defenderName;
            break;

        case RollType::Defense:
            m_infoMessage =
                m_defenderName +
                "\nRolling Defense Roll\nagainst " +
                m_attackerName;
            break;

        case RollType::None:
        default:
            m_infoMessage.clear();
            break;
        }
    }

    void CombatPresentation::beginResult(
        RollType rollType)
    {
        m_phase = Phase::Result;
        m_rollType = rollType;
        m_phaseElapsed = 0.0f;

        const AttackResult* result =
            currentAttack();

        if (result == nullptr)
        {
            m_infoMessage.clear();
            return;
        }

        switch (rollType)
        {
        case RollType::Precision:
            m_infoMessage =
                m_attackerName +
                "\nPrecision Roll " +
                (precisionPassed() ? "Passed" : "Failed") +
                "\nagainst " +
                m_defenderName;
            break;

        case RollType::Wound:
            if (woundPassed())
            {
                if (result->resolution ==
                    AttackResolution::Defeated)
                {
                    m_infoMessage =
                        m_attackerName +
                        "\nWound Roll Passed\nagainst " +
                        m_defenderName +
                        "\nTarget defeated!";
                }
                else
                {
                    m_infoMessage =
                        m_attackerName +
                        "\nWound Roll Passed\nagainst " +
                        m_defenderName +
                        "\nHit for " +
                        std::to_string(result->damage) +
                        " damage.";
                }
            }
            else
            {
                m_infoMessage =
                    m_attackerName +
                    "\nWound Roll Failed\nagainst " +
                    m_defenderName;
            }
            break;

        case RollType::Defense:
            if (result->resolution ==
                AttackResolution::Defended)
            {
                m_infoMessage =
                    m_defenderName +
                    "\nDefense Roll Passed\nagainst " +
                    m_attackerName +
                    "\nAttack blocked.";
            }
            else
            {
                if (result->resolution ==
                    AttackResolution::Defeated)
                {
                    m_infoMessage =
                        m_defenderName +
                        "\nDefense Roll Failed\nagainst " +
                        m_attackerName +
                        "\nTarget defeated!";
                }
                else
                {
                    m_infoMessage =
                        m_defenderName +
                        "\nDefense Roll Failed\nagainst " +
                        m_attackerName +
                        "\nHit for " +
                        std::to_string(result->damage) +
                        " damage.";
                }
            }
            break;

        case RollType::None:
        default:
            m_infoMessage.clear();
            break;
        }
    }

    void CombatPresentation::advanceAfterResult()
    {
        const AttackResult* result =
            currentAttack();

        if (result == nullptr)
        {
            m_phase = Phase::None;
            m_rollType = RollType::None;
            m_infoMessage.clear();
            m_attackerName.clear();
            m_defenderName.clear();
            return;
        }

        bool advanceToNextAttack = false;

        switch (m_rollType)
        {
        case RollType::Precision:
            // A failed Precision roll ends this attack.
            if (!precisionPassed())
            {
                advanceToNextAttack = true;
            }
            else
            {
                beginRolling(RollType::Wound);
                return;
            }
            break;

        case RollType::Wound:
            // A failed Wound roll ends this attack.
            if (!woundPassed())
            {
                advanceToNextAttack = true;
            }
            else if (hasDefenseRoll())
            {
                beginRolling(RollType::Defense);
                return;
            }
            else
            {
                advanceToNextAttack = true;
            }
            break;

        case RollType::Defense:
            advanceToNextAttack = true;
            break;

        case RollType::None:
        default:
            advanceToNextAttack = true;
            break;
        }

        if (!advanceToNextAttack)
        {
            return;
        }

        ++m_attackIndex;

        if (m_attackIndex >= m_attacks.size())
        {
            m_phase = Phase::None;
            m_rollType = RollType::None;
            m_infoMessage.clear();
            m_attackerName.clear();
            m_defenderName.clear();
            return;
        }

        beginRolling(RollType::Precision);
    }

    bool CombatPresentation::precisionPassed() const noexcept
    {
        const AttackResult* result =
            currentAttack();

        if (result == nullptr)
        {
            return false;
        }

        return result->precisionRoll >=
            result->precisionTarget;
    }

    bool CombatPresentation::woundPassed() const noexcept
    {
        const AttackResult* result =
            currentAttack();

        if (result == nullptr)
        {
            return false;
        }

        return result->woundRoll >=
            result->woundTarget;
    }

    bool CombatPresentation::hasDefenseRoll() const noexcept
    {
        const AttackResult* result =
            currentAttack();

        if (result == nullptr)
        {
            return false;
        }

        return result->defenseRoll > 0 &&
            result->defenseTarget > 0;
    }
}