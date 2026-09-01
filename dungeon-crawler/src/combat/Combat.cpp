#include "combat/Combat.h"

#include "combat/CombatRules.h"
#include "combat/CombatTarget.h"
#include "combat/IDice.h"
#include "entities/Player.h"

#include <sstream>
#include <stdexcept>

namespace dungeon
{
    Combat::Combat(
        Player& player,
        CombatTarget& target,
        IDice& dice)
        : m_player(player),
        m_target(target),
        m_dice(dice)
    {
        if (&player == &target)
        {
            throw std::invalid_argument(
                "Player cannot fight itself.");
        }
    }

    CombatResult Combat::playerAttack()
    {
        if (!isActive())
        {
            return {
                m_target.isDefeated(),
                m_player.isDefeated(),
                "Combat is no longer active."
            };
        }

        m_lastPlayerAttacks.clear();

        const CombatStats playerStats =
            m_player.combatStats();

        return resolveAttacks(
            m_player,
            m_target,
            m_player.weaponDamage(),
            m_lastPlayerAttacks);
    }

    CombatResult Combat::enemyTurn()
    {
        if (!isActive())
        {
            return {
                m_target.isDefeated(),
                m_player.isDefeated(),
                "Combat is no longer active."
            };
        }

        m_lastEnemyAttacks.clear();

        const CombatStats targetStats =
            m_target.combatStats();

        if (targetStats.attacks <= 0)
        {
            return {
                false,
                false,
                "The target has no attacks."
            };
        }

        return resolveAttacks(
            m_target,
            m_player,
            1,
            m_lastEnemyAttacks);
    }

    CombatResult Combat::escape()
    {
        if (!isActive())
        {
            return {
                m_target.isDefeated(),
                m_player.isDefeated(),
                "Combat is no longer active."
            };
        }

        m_active = false;
        m_playerEscaped = true;

        return {
            false,
            false,
            "Escaped from combat."
        };
    }

    bool Combat::isActive() const noexcept
    {
        return m_active;
    }

    bool Combat::playerWon() const noexcept
    {
        return !m_active &&
            !m_playerEscaped &&
            m_target.isDefeated();
    }

    bool Combat::playerLost() const noexcept
    {
        return !m_active &&
            !m_playerEscaped &&
            m_player.isDefeated();
    }

    bool Combat::escaped() const noexcept
    {
        return m_playerEscaped;
    }

    CombatTarget& Combat::target() noexcept
    {
        return m_target;
    }

    const CombatTarget& Combat::target() const noexcept
    {
        return m_target;
    }

    const std::vector<AttackResult>&
        Combat::lastPlayerAttacks() const noexcept
    {
        return m_lastPlayerAttacks;
    }

    const std::vector<AttackResult>&
        Combat::lastEnemyAttacks() const noexcept
    {
        return m_lastEnemyAttacks;
    }

    AttackResult Combat::resolveAttack(
        CombatTarget& attacker,
        CombatTarget& defender,
        int damage)
    {
        const CombatStats attackerStats =
            attacker.combatStats();

        const CombatStats defenderStats =
            defender.combatStats();

        AttackResult result;

        result.precisionTarget =
            attackerStats.precision;

        result.precisionRoll =
            m_dice.rollD6();

        if (!CombatRules::passesRoll(
            result.precisionRoll,
            result.precisionTarget))
        {
            result.resolution =
                AttackResolution::Missed;

            return result;
        }

        result.woundTarget =
            CombatRules::woundTarget(
                attackerStats.strength,
                defenderStats.toughness);

        result.woundRoll =
            m_dice.rollD6();

        if (!CombatRules::passesRoll(
            result.woundRoll,
            result.woundTarget))
        {
            result.resolution =
                AttackResolution::FailedToWound;

            return result;
        }

        result.defenseTarget =
            defenderStats.defense;

        result.defenseRoll =
            m_dice.rollD6();

        if (CombatRules::passesRoll(
            result.defenseRoll,
            result.defenseTarget))
        {
            result.resolution =
                AttackResolution::Defended;

            return result;
        }

        defender.takeDamage(damage);

        result.damage = damage;

        if (defender.isDefeated())
        {
            result.resolution =
                AttackResolution::Defeated;
        }
        else
        {
            result.resolution =
                AttackResolution::Damaged;
        }

        return result;
    }

    CombatResult Combat::resolveAttacks(
        CombatTarget& attacker,
        CombatTarget& defender,
        int damage,
        std::vector<AttackResult>& results)
    {
        const CombatStats attackerStats =
            attacker.combatStats();

        for (int attack = 0;
            attack < attackerStats.attacks;
            ++attack)
        {
            if (defender.isDefeated())
            {
                break;
            }

            results.push_back(
                resolveAttack(
                    attacker,
                    defender,
                    damage));
        }

        const bool defenderDefeated =
            defender.isDefeated();

        const bool attackerDefeated =
            attacker.isDefeated();

        if (defenderDefeated ||
            attackerDefeated)
        {
            m_active = false;
        }

        std::ostringstream message;

        message
            << results.size()
            << " attack";

        if (results.size() != 1)
        {
            message << "s";
        }

        message << " resolved.";

        if (defenderDefeated)
        {
            message << " Target defeated.";
        }

        if (attackerDefeated)
        {
            message << " Player defeated.";
        }

        return {
            defenderDefeated,
            attackerDefeated,
            message.str()
        };
    }
}