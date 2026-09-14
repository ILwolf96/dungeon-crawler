#pragma once

#include "combat/CombatStats.h"
#include "combat/CombatPresentation.h"
#include "entities/Chest.h"
#include "entities/Enemy.h"
#include "entities/Player.h"
#include "world/Map.h"
#include "combat/Combat.h"
#include "combat/RandomDice.h"
#include "config/ConfigData.h"
#include "loot/LootReward.h"

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace dungeon
{
    enum class Action
    {
        None,
        MoveUp,
        MoveDown,
        MoveLeft,
        MoveRight,
        TurnLeft,
        TurnRight,
        Attack,
        Inventory,
        Escape,
        ConfirmYes,
        ConfirmNo,
        LootContinue
    };

    enum class FacingDirection
    {
        North,
        East,
        South,
        West
    };

    enum class CombatLootPreviewType
    {
        None,
        Weapon,
        Armor,
        Accessory,
        HealthPotion,
        RagePotion
    };

    enum class GameFlowResult
    {
        None,
        ExitApplication,
        RestartGame
    };

    struct CombatLootPreviewSlot
    {
        CombatLootPreviewType type{ CombatLootPreviewType::None };
        std::string id;
        int tier{ 0 };
    };

    class Game
    {
    public:
        Game();

        void load(std::string_view filePath);
        void restart();

        void handleAction(Action action);
        GameFlowResult handleOutcomeAction(Action action);

        [[nodiscard]]
        bool lootPromptActive() const noexcept;

        [[nodiscard]]
        const std::string& lootMessage() const noexcept;

        void update(float deltaSeconds);

        bool isZoo() const noexcept;

        [[nodiscard]]
        FacingDirection facingDirection() const noexcept;

        bool inCombat() const noexcept;

        Combat* combat() noexcept;
        const Combat* combat() const noexcept;

        void startCombat(CombatTarget& target);

        int displayedPlayerHp() const noexcept;
        int displayedCombatTargetHp() const noexcept;

        [[nodiscard]]
        int windowWidth() const noexcept;

        [[nodiscard]]
        int windowHeight() const noexcept;

        [[nodiscard]]
        const std::string& title() const noexcept;

        [[nodiscard]]
        const Map& map() const noexcept;

        [[nodiscard]]
        Player& player() noexcept;

        [[nodiscard]]
        const Player& player() const noexcept;

        [[nodiscard]]
        const std::vector<std::unique_ptr<Enemy>>& enemies() const noexcept;

        [[nodiscard]]
        const std::vector<std::unique_ptr<Chest>>& chests() const noexcept;

        [[nodiscard]]
        bool combatPresentationActive() const noexcept;

        [[nodiscard]]
        const CombatPresentation& combatPresentation() const noexcept;

        [[nodiscard]]
        bool combatPresentationPlayerIsAttacking() const noexcept;

        [[nodiscard]]
        const std::vector<CombatLootPreviewSlot>& combatLootPreview() const noexcept;

        bool inventoryOpen() const noexcept;
        const std::string& combatInfoMessage() const noexcept;

        // -----------------------------------------------------------------
        // Win / Loss State
        // -----------------------------------------------------------------

        [[nodiscard]]
        bool portalPromptActive() const noexcept;

        [[nodiscard]]
        bool defeatPromptActive() const noexcept;

        [[nodiscard]]
        std::string_view defeatedEnemyType() const noexcept;

    private:
        Map m_map;
        Player m_player;

        CombatTarget* combatTargetAt(int x, int y) noexcept;
        void finishCombatIfNeeded();
        void performCombatAttack();
        void performCombatEscape();
        void openCombatInventory();
        void closeCombatInventory();
        void advanceCombatPresentation();
        void beginEnemyTurnPresentation();
        void closeLootPrompt();
        void handleZooInteraction(char tile);

        std::vector<CombatLootPreviewSlot> buildCombatLootPreview(
            const CombatTarget& target,
            int lootTier) const;

        std::vector<std::unique_ptr<Enemy>> m_enemies;
        std::vector<std::unique_ptr<Chest>> m_chests;
        std::vector<CombatLootPreviewSlot> m_combatLootPreview;
        std::unique_ptr<Combat> m_combat;
        CombatPresentation m_combatPresentation;

        enum class CombatPresentationActor
        {
            None,
            Player,
            Enemy
        };

        int m_displayedPlayerHp{ 0 };
        int m_displayedCombatTargetHp{ 0 };
        CombatPresentationActor m_combatPresentationActor{
            CombatPresentationActor::None
        };

        std::size_t m_lastPresentedDamageAttackIndex{
         static_cast<std::size_t>(-1)
        };

        CombatPresentation::RollType m_lastPresentedDamageRollType{
            CombatPresentation::RollType::None
        };

        RandomDice m_combatDice;
        bool m_inventoryOpen{ false };
        bool m_pendingEnemyTurn{ false };
        bool m_pendingCombatFinish{ false };
        bool m_pendingPotionEnemyTurn{ false };

        std::string m_combatInfoMessage;

        float m_combatInfoMessageTime{ 0.0f };

        int m_healthPotionRestoreAmount{ 0 };
        int m_ragePotionDamageBonus{ 0 };
        bool useHealthPotion();
        bool useRagePotion();

        config::ConfigData m_configData;

        bool m_isZoo{ false };
        FacingDirection m_facingDirection{ FacingDirection::North };

        int m_windowWidth{ 640 };
        int m_windowHeight{ 360 };
        std::string m_title{ "Dungeon Crawler" };

        // -----------------------------------------------------------------
        // Win / Loss state
        // -----------------------------------------------------------------

        bool m_portalPromptActive{ false };

        int m_portalPreviousPlayerX{ 0 };
        int m_portalPreviousPlayerY{ 0 };

        bool m_defeatPromptActive{ false };
        std::string m_defeatedEnemyType;

        bool m_lootPromptActive{ false };
        std::string m_lootMessage;

        std::string m_configFilePath;
    };
}