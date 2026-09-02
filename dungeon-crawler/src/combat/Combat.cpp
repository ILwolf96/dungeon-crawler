#include "combat/Combat.h"

#include "combat/CombatRules.h"
#include "combat/CombatTarget.h"
#include "combat/IDice.h"
#include "entities/Player.h"

#include <iostream>
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
        m_dice(dice),
        m_playerDamageBonus(0)
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

        const CombatStats attackerStats =
            m_player.combatStats();

        std::vector<AttackResult>& results = m_lastPlayerAttacks;
        const int baseDamage = m_player.weaponDamage();
        const int damage = baseDamage + m_playerDamageBonus;

        std::clog
            << "[COMBAT] Player weapon damage: "
            << baseDamage
            << " + temporary bonus "
            << m_playerDamageBonus
            << " = "
            << damage
            << '\n';

        std::clog << "[COMBAT] Player attacks " << m_target.targetType() << ".\n";

        for (int attack = 0;
            attack < attackerStats.attacks;
            ++attack)
        {
            if (m_target.isDefeated())
            {
                break;
            }

            const int targetHpBefore = m_target.currentHp();

            results.push_back(
                resolveAttack(
                    m_player,
                    m_target,
                    damage));

            const int targetHpAfter = m_target.currentHp();

            std::clog
                << "[COMBAT] "
                << m_target.targetType()
                << " HP: "
                << targetHpBefore
                << " -> "
                << targetHpAfter
                << " / "
                << m_target.maxHp()
                << '\n';
        }

        const bool defenderDefeated =
            m_target.isDefeated();

        const bool attackerDefeated =
            m_player.isDefeated();

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

        std::clog << '\n';

        return {
            defenderDefeated,
            attackerDefeated,
            message.str()
        };
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

        std::clog << "[COMBAT] " << m_target.targetType() << " attacks Player.\n";

        const CombatStats attackerStats =
            m_target.combatStats();

        std::vector<AttackResult>& results = m_lastEnemyAttacks;
        int damage = 1;

        for (int attack = 0;
            attack < attackerStats.attacks;
            ++attack)
        {
            if (m_player.isDefeated())
            {
                break;
            }

            const int playerHpBefore = m_player.currentHp();

            results.push_back(
                resolveAttack(
                    m_target,
                    m_player,
                    damage));

            const int playerHpAfter = m_player.currentHp();

            std::clog
                << "[COMBAT] Player HP: "
                << playerHpBefore
                << " -> "
                << playerHpAfter
                << " / "
                << m_player.maxHp()
                << '\n';
        }

        const bool defenderDefeated =
            m_player.isDefeated();

        const bool attackerDefeated =
            m_target.isDefeated();

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
            message << " Player defeated.";
        }

        if (attackerDefeated)
        {
            message << " Target defeated.";
        }

        std::clog << '\n';

        return {
            attackerDefeated,
            defenderDefeated,
            message.str()
        };
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

        std::clog << "[COMBAT] Escaped from combat.\n\n";

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

    void Combat::addPlayerDamageBonus(int amount)
    {
        if (amount <= 0)
        {
            return;
        }

        m_playerDamageBonus += amount;

        std::clog
            << "[COMBAT] Player temporary damage bonus increased by "
            << amount
            << ". Current bonus: +"
            << m_playerDamageBonus
            << '\n';
    }

    int Combat::playerDamageBonus() const noexcept
    {
        return m_playerDamageBonus;
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
        }
        else
        {
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
            }
            else
            {
                if (!defender.canDefend())
                {
                    result.damage = damage;
                    defender.takeDamage(result.damage);

                    if (defender.isDefeated())
                    {
                        result.resolution = AttackResolution::Defeated;
                    }
                    else
                    {
                        result.resolution = AttackResolution::Damaged;
                    }

                    std::clog
                        << "[COMBAT]   - Precision: " << result.precisionRoll << " / " << result.precisionTarget << " (PASSED)\n"
                        << "[COMBAT]   - Wound:     " << result.woundRoll << " / " << result.woundTarget << " (PASSED)\n"
                        << "[COMBAT]   - Defense:   " << defender.targetType() << " cannot defend.\n"
                        << "[COMBAT]   - Result:    Hit! Damage=" << result.damage << '\n';

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
                }
                else
                {
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
                }
            }
        }

        // Expanded multi-line logging showing each entity's roll and outcome step-by-step, this is tempery until I manage to get the UI running
        switch (result.resolution)
        {
        case AttackResolution::Missed:
            std::clog
                << "[COMBAT]   - Precision: " << result.precisionRoll << " / " << result.precisionTarget << " (FAILED)\n"
                << "[COMBAT]   - Result:    Precision failed.\n";
            break;
        case AttackResolution::FailedToWound:
            std::clog
                << "[COMBAT]   - Precision: " << result.precisionRoll << " / " << result.precisionTarget << " (PASSED)\n"
                << "[COMBAT]   - Wound:     " << result.woundRoll << " / " << result.woundTarget << " (FAILED)\n"
                << "[COMBAT]   - Result:    Wound failed.\n";
            break;
        case AttackResolution::Defended:
            std::clog
                << "[COMBAT]   - Precision: " << result.precisionRoll << " / " << result.precisionTarget << " (PASSED)\n"
                << "[COMBAT]   - Wound:     " << result.woundRoll << " / " << result.woundTarget << " (PASSED)\n"
                << "[COMBAT]   - Defense:   " << result.defenseRoll << " / " << result.defenseTarget << " (BLOCKED)\n"
                << "[COMBAT]   - Result:    Defense succeeded. Damage=0\n";
            break;
        case AttackResolution::Damaged:
            std::clog
                << "[COMBAT]   - Precision: " << result.precisionRoll << " / " << result.precisionTarget << " (PASSED)\n"
                << "[COMBAT]   - Wound:     " << result.woundRoll << " / " << result.woundTarget << " (PASSED)\n"
                << "[COMBAT]   - Defense:   " << result.defenseRoll << " / " << result.defenseTarget << " (PIERCED)\n"
                << "[COMBAT]   - Result:    Hit! Damage=" << result.damage << '\n';
            break;
        case AttackResolution::Defeated:
            std::clog
                << "[COMBAT]   - Precision: " << result.precisionRoll << " / " << result.precisionTarget << " (PASSED)\n"
                << "[COMBAT]   - Wound:     " << result.woundRoll << " / " << result.woundTarget << " (PASSED)\n"
                << "[COMBAT]   - Defense:   " << result.defenseRoll << " / " << result.defenseTarget << " (PIERCED)\n"
                << "[COMBAT]   - Result:    Target defeated! Damage=" << result.damage << '\n';
            break;
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
            message << " Attacker defeated.";
        }

        return {
            defenderDefeated,
            attackerDefeated,
            message.str()
        };
    }
}