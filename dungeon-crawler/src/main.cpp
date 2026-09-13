#include "app/Game.h"
#include "app/LaunchMenu.h"
#include "rendering/LaunchMenuRenderer.h"
#include "rendering/Renderer.h"

// Testing! Don't forget to remove when done!
#include "combat/Combat.h"
#include "combat/SequenceDice.h"
//

#include "raylib.h"

#include <iostream>
#include <exception>

namespace
{
    dungeon::Action pollAction(const dungeon::Game& game)
    {
        if (IsKeyPressed(KEY_Q) && !game.inCombat())
        {
            return dungeon::Action::TurnLeft;
        }

        if (IsKeyPressed(KEY_E) && !game.inCombat())
        {
            return dungeon::Action::TurnRight;
        }

        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
        {
            return dungeon::Action::MoveUp;
        }

        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
        {
            return dungeon::Action::MoveDown;
        }

        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
        {
            return dungeon::Action::MoveLeft;
        }

        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
        {
            return dungeon::Action::MoveRight;
        }

        return dungeon::Action::None;
    }

    // TESTING! DON'T FORGET TO UPDATE WHEN DONE!
    void runCombatDiagnostic(dungeon::Game& game)
    {
        const auto& enemies = game.enemies();

        if (enemies.empty())
        {
            TraceLog(
                LOG_ERROR,
                "Combat diagnostic failed: no enemies are loaded.");

            return;
        }

        dungeon::SequenceDice dice{
            {
                4,
                5,
                2,
                1
            }
        };

        dungeon::Combat combat(
            game.player(),
            *enemies.front(),
            dice);

        TraceLog(
            LOG_INFO,
            "Enemy HP before attack: %d / %d",
            enemies.front()->currentHp(),
            enemies.front()->maxHp());

        const dungeon::CombatResult result =
            combat.playerAttack();

        TraceLog(
            LOG_INFO,
            "Combat diagnostic: %s",
            result.message.c_str());

        const auto& attacks =
            combat.lastPlayerAttacks();

        if (attacks.empty())
        {
            TraceLog(
                LOG_ERROR,
                "Combat diagnostic produced no attack result.");

            return;
        }

        const auto& attack = attacks.front();

        TraceLog(
            LOG_INFO,
            "Precision: roll %d / target %d",
            attack.precisionRoll,
            attack.precisionTarget);

        TraceLog(
            LOG_INFO,
            "Wound: roll %d / target %d",
            attack.woundRoll,
            attack.woundTarget);

        TraceLog(
            LOG_INFO,
            "Defense: roll %d / target %d",
            attack.defenseRoll,
            attack.defenseTarget);

        TraceLog(
            LOG_INFO,
            "Damage: %d",
            attack.damage);

        TraceLog(
            LOG_INFO,
            "Enemy HP: %d / %d",
            enemies.front()->currentHp(),
            enemies.front()->maxHp());
    }
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    dungeon::LaunchMenu launchMenu;
    dungeon::LaunchMenuRenderer launchMenuRenderer;

    InitWindow(
        dungeon::LaunchMenu::windowWidth(),
        dungeon::LaunchMenu::windowHeight(),
        "Dungeon Crawler - Launch Menu");

    if (!launchMenuRenderer.loadAssets())
    {
        TraceLog(
            LOG_WARNING,
            "Some Launch Menu assets are not available. "
            "Raylib fallback rendering will remain active for missing assets.");
    }

    SetTargetFPS(60);

    dungeon::LaunchMenuResult launchResult =
        dungeon::LaunchMenuResult::None;

    while (!WindowShouldClose() &&
        launchResult == dungeon::LaunchMenuResult::None)
    {
        if (IsKeyPressed(KEY_ONE))
        {
            launchMenu.handleAction(1);
        }
        else if (IsKeyPressed(KEY_TWO))
        {
            launchMenu.handleAction(2);
        }
        else if (IsKeyPressed(KEY_THREE))
        {
            launchMenu.handleAction(3);
        }
        else if (IsKeyPressed(KEY_FOUR))
        {
            launchMenu.handleAction(4);
        }

        launchResult =
            launchMenu.update(GetFrameTime());

        BeginDrawing();

        launchMenuRenderer.draw(launchMenu);

        EndDrawing();
    }

    launchMenuRenderer.unloadAssets();
    CloseWindow();

    if (launchResult != dungeon::LaunchMenuResult::Launch)
    {
        return 0;
    }

    dungeon::Game game;
    dungeon::Renderer renderer;

    try
    {
        game.load(launchMenu.configFilePath());
    }
    catch (const std::exception& error)
    {
        std::cerr
            << "Error: Failed to load configuration.\n"
            << error.what()
            << '\n';

        return 1;
    }

    InitWindow(
        game.windowWidth(),
        game.windowHeight(),
        game.title().c_str());

    if (launchMenu.graphicPreview() == dungeon::LaunchGraphicPreview::Png)
    {
        if (!renderer.loadMainScreenAssets())
        {
            TraceLog(
                LOG_WARNING,
                "Some Main Screen assets are not available yet. "
                "Raylib fallback rendering will remain active for missing assets.");
        }
    }
    else
    {
        TraceLog(
            LOG_INFO,
            "Raylib graphics selected."
            "Main Screen PNG assets will not be loaded.");
    }

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        dungeon::GameFlowResult flowResult =
            dungeon::GameFlowResult::None;

        // -------------------------------------------------------------
        // Portal / Defeat result screen
        // -------------------------------------------------------------

        if (game.portalPromptActive() || game.defeatPromptActive())
        {
            if (IsKeyPressed(KEY_Y))
            {
                flowResult = game.handleOutcomeAction(dungeon::Action::ConfirmYes);
            }
            else if (IsKeyPressed(KEY_N))
            {
                flowResult = game.handleOutcomeAction(dungeon::Action::ConfirmNo);
            }
        }
        else
        {
            // ---------------------------------------------------------
            // Game Instructions
            // ---------------------------------------------------------

            if (IsKeyPressed(KEY_TAB))
            {
                renderer.toggleGameInstructions();
            }

            // ---------------------------------------------------------
            // Attack
            // ---------------------------------------------------------

            if (IsKeyPressed(KEY_F1) || IsKeyPressed(KEY_ONE))
            {
                game.handleAction(dungeon::Action::Attack);
            }

            // ---------------------------------------------------------
            // Inventory
            // ---------------------------------------------------------

            if (IsKeyPressed(KEY_I) || IsKeyPressed(KEY_TWO))
            {
                game.handleAction(dungeon::Action::Inventory);
            }

            // ---------------------------------------------------------
            // Escape / Close Inventory
            // ---------------------------------------------------------

            if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_THREE))
            {
                if (game.inCombat())
                {
                    game.handleAction(dungeon::Action::Escape);
                }
            }

            const dungeon::Action action = pollAction(game);

            if (action != dungeon::Action::None)
            {
                game.handleAction(action);
            }
        }

        // -------------------------------------------------------------
        // application/game flow
        // -------------------------------------------------------------

        if (flowResult == dungeon::GameFlowResult::ExitApplication)
        {
            break;
        }

        if (flowResult == dungeon::GameFlowResult::RestartGame)
        {
            try
            {
                game.restart();
                renderer.resetMapCamera();
            }
            catch (const std::exception& error)
            {
                std::cerr
                    << "Error: Failed to restart the game.\n"
                    << error.what() //da fk :/
                    << '\n';

                break;
            }
        }

        //Updates Game here!!!
        game.update(GetFrameTime());

        BeginDrawing();

        renderer.draw(game);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}