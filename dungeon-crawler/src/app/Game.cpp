#include "app/Game.h"
#include "rendering/Renderer.h"
#include "entities/EnemyFactory.h"
#include "entities/Chest.h"
#include "config/ParserFactory.h"

#include "gear/GearFactory.h"
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace
{
    constexpr std::string_view EnemySectionPrefix = "enemy.";

    bool isEnemyDefinition(std::string_view sectionName)
    {
        return sectionName.starts_with(EnemySectionPrefix);
    }

    int getRequiredInt(
        const config::ConfigData& data,
        std::string_view section,
        std::string_view key)
    {
        const std::string sectionName(section);
        const std::string keyName(key);

        if (!data.hasValue(sectionName, keyName))
        {
            throw std::runtime_error(
                "Missing configuration value: " +
                sectionName + "." +
                keyName);
        }

        try
        {
            return std::stoi(
                data.getValue(sectionName, keyName));
        }
        catch (const std::exception&)
        {
            throw std::runtime_error(
                "Invalid integer value for: " +
                sectionName + "." +
                keyName);
        }
    }

    std::string getRequiredString(
        const config::ConfigData& data,
        std::string_view section,
        std::string_view key)
    {
        const std::string sectionName(section);
        const std::string keyName(key);

        if (!data.hasValue(sectionName, keyName))
        {
            throw std::runtime_error(
                "Missing configuration value: " +
                sectionName + "." +
                keyName);
        }

        const std::string value =
            data.getValue(sectionName, keyName);

        if (value.empty())
        {
            throw std::runtime_error(
                "Configuration value cannot be empty: " +
                sectionName + "." +
                keyName);
        }

        return value;
    }

    char getRequiredSymbol(
        const config::ConfigData& data,
        std::string_view section)
    {
        const std::string symbol =
            getRequiredString(data, section, "symbol");

        if (symbol.size() != 1)
        {
            throw std::runtime_error(
                "Enemy symbol must contain exactly one character: " +
                std::string(section));
        }

        return symbol.front();
    }
}

namespace dungeon
{
    Game::Game()
        : m_map(),
        m_player(0, 0)
    {
    }

    void Game::load(std::string_view filePath)
    {
        auto parser = config::makeParser(filePath);
        const config::ConfigData data = parser->parse(filePath);

        // Window configuration.
        const int windowWidth =
            getRequiredInt(data, "window", "width");

        const int windowHeight =
            getRequiredInt(data, "window", "height");

        const std::string title =
            getRequiredString(data, "window", "title");

        if (windowWidth <= 0 || windowHeight <= 0)
        {
            throw std::runtime_error(
                "Window width and height must be greater than zero.");
        }

        // Map configuration.
        const std::size_t height =
            static_cast<std::size_t>(
                getRequiredInt(data, "map", "height"));

        if (height == 0)
        {
            throw std::runtime_error(
                "Map height must be greater than zero.");
        }

        Map::Grid grid;
        grid.reserve(height);

        for (std::size_t row = 0; row < height; ++row)
        {
            const std::string key =
                std::string("row_") +
                (row < 10 ? "0" : "") +
                std::to_string(row);

            grid.push_back(
                getRequiredString(data, "map", key));
        }

        m_map.setGrid(std::move(grid));

        // Build a lookup from map symbol to enemy definition.
        std::unordered_map<char, std::string> enemyDefinitions;

        for (const auto& [sectionName, sectionData] :
            data.sections())
        {
            if (!isEnemyDefinition(sectionName))
            {
                continue;
            }

            const char symbol =
                getRequiredSymbol(data, sectionName);

            if (symbol == '#' ||
                symbol == '.' ||
                symbol == 'P' ||
                symbol == 'C')
            {
                throw std::runtime_error(
                    "Enemy symbol conflicts with a reserved map symbol: " +
                    std::string(1, symbol));
            }

            if (enemyDefinitions.contains(symbol))
            {
                throw std::runtime_error(
                    "Duplicate enemy symbol: " +
                    std::string(1, symbol));
            }

            enemyDefinitions.emplace(
                symbol,
                sectionName);
        }

        // Locate the player and create entities from the map.
        bool playerFound = false;
        m_enemies.clear();
        m_chests.clear();

        for (std::size_t y = 0; y < m_map.height(); ++y)
        {
            for (std::size_t x = 0; x < m_map.width(); ++x)
            {
                const char tile =
                    m_map.tileAt(
                        static_cast<int>(x),
                        static_cast<int>(y));

                if (tile == 'P')
                {
                    if (playerFound)
                    {
                        throw std::runtime_error(
                            "Map contains more than one player.");
                    }

                    playerFound = true;

                    m_player.setPosition(
                        static_cast<int>(x),
                        static_cast<int>(y));

                    continue;
                }

                if (tile == 'C')
                {
                    m_chests.push_back(
                        std::make_unique<Chest>(
                            static_cast<int>(x),
                            static_cast<int>(y)));

                    continue;
                }

                const auto enemyDefinition =
                    enemyDefinitions.find(tile);

                if (enemyDefinition == enemyDefinitions.end())
                {
                    continue;
                }

                const std::string& sectionName =
                    enemyDefinition->second;

                const std::string type =
                    sectionName.substr(EnemySectionPrefix.size());

                const int maxHp =
                    getRequiredInt(
                        data,
                        sectionName,
                        "max_hp");

                const int tier =
                    getRequiredInt(
                        data,
                        sectionName,
                        "tier");

                const CombatStats stats{
                    getRequiredInt(
                        data,
                        sectionName,
                        "attacks"),

                    getRequiredInt(
                        data,
                        sectionName,
                        "precision"),

                    getRequiredInt(
                        data,
                        sectionName,
                        "strength"),

                    getRequiredInt(
                        data,
                        sectionName,
                        "toughness"),

                    getRequiredInt(
                        data,
                        sectionName,
                        "defense")
                };

                auto enemy = EnemyFactory::create(
                    type,
                    maxHp,
                    tier,
                    stats,
                    static_cast<int>(x),
                    static_cast<int>(y));

                m_enemies.push_back(
                    std::move(enemy));
            }
        }

        if (!playerFound)
        {
            throw std::runtime_error(
                "Map does not contain a player.");
        }

        // Player configuration.
        const int playerMaxHp =
            getRequiredInt(data, "player", "max_hp");

        const CombatStats playerBaseStats{
            getRequiredInt(data, "player", "attacks"),
            getRequiredInt(data, "player", "precision"),
            getRequiredInt(data, "player", "strength"),
            getRequiredInt(data, "player", "toughness"),
            getRequiredInt(data, "player", "defense")
        };

        const std::string weaponId =
            getRequiredString(data, "player", "weapon");

        const std::string armorId =
            getRequiredString(data, "player", "armor");

        auto playerWeapon =
            GearFactory::createWeapon(
                data,
                weaponId);

        auto playerArmor =
            GearFactory::createArmor(
                data,
                armorId);

        m_player.initialize(
            playerMaxHp,
            playerBaseStats,
            std::move(playerWeapon),
            std::move(playerArmor));

        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;
        m_title = title;
    }

    void Game::handleAction(Action action)
    {
        if (m_combat)
        {
            if (action == Action::Attack)
            {
                performCombatAttack();
            }

            return;
        }

        int targetX = m_player.x();
        int targetY = m_player.y();

        switch (action)
        {
        case Action::MoveUp:
            --targetY;
            break;

        case Action::MoveDown:
            ++targetY;
            break;

        case Action::MoveLeft:
            --targetX;
            break;

        case Action::MoveRight:
            ++targetX;
            break;

        case Action::None:
            return;
        }

        if (!m_map.isWalkable(targetX, targetY))
        {
            return;
        }

        m_player.setPosition(targetX, targetY);

        CombatTarget* target = combatTargetAt(targetX, targetY);

        if (target != nullptr)
        {
            startCombat(*target);
        }
    }

    bool Game::inCombat() const noexcept
    {
        return m_combat != nullptr;
    }

    Combat* Game::combat() noexcept
    {
        return m_combat.get();
    }

    const Combat* Game::combat() const noexcept
    {
        return m_combat.get();
    }

    void Game::startCombat(CombatTarget& target)
    {
        if (m_combat)
        {
            return;
        }

        m_combat = std::make_unique<Combat>(
            m_player,
            target,
            m_combatDice);
    }

    CombatTarget* Game::combatTargetAt(int x, int y) noexcept
    {
        for (const auto& enemy : m_enemies)
        {
            if (enemy->x() == x && enemy->y() == y &&
                !enemy->isDefeated())
            {
                return enemy.get();
            }
        }

        for (const auto& chest : m_chests)
        {
            if (chest->x() == x && chest->y() == y &&
                !chest->isDefeated())
            {
                return chest.get();
            }
        }

        return nullptr;
    }

    void Game::performCombatAttack()
    {
        if (!m_combat)
        {
            return;
        }

        m_combat->playerAttack();

        if (!m_combat->isActive())
        {
            finishCombatIfNeeded();
            return;
        }

        m_combat->enemyTurn();

        if (!m_combat->isActive())
        {
            finishCombatIfNeeded();
        }
    }

    void Game::finishCombatIfNeeded()
    {
        if (!m_combat)
        {
            return;
        }

        if (!m_combat->isActive())
        {
            CombatTarget* target = &m_combat->target();

            const bool targetDefeated =
                target->isDefeated();

            m_combat.reset();

            if (!targetDefeated)
            {
                return;
            }

            for (auto it = m_enemies.begin();
                it != m_enemies.end();
                ++it)
            {
                if (it->get() == target)
                {
                    m_enemies.erase(it);
                    return;
                }
            }

            for (auto it = m_chests.begin();
                it != m_chests.end();
                ++it)
            {
                if (it->get() == target)
                {
                    m_chests.erase(it);
                    return;
                }
            }
        }
    }

    int Game::windowWidth() const noexcept
    {
        return m_windowWidth;
    }

    int Game::windowHeight() const noexcept
    {
        return m_windowHeight;
    }

    const std::string& Game::title() const noexcept
    {
        return m_title;
    }

    const Map& Game::map() const noexcept
    {
        return m_map;
    }

    Player& Game::player() noexcept
    {
        return m_player;
    }

    const Player& Game::player() const noexcept
    {
        return m_player;
    }

    const std::vector<std::unique_ptr<Enemy>>& Game::enemies() const noexcept
    {
        return m_enemies;
    }

    const std::vector<std::unique_ptr<Chest>>& Game::chests() const noexcept
    {
        return m_chests;
    }
}