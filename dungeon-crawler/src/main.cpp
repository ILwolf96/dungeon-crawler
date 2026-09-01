#include "app/Game.h"
#include "rendering/Renderer.h"

//Testing! don't forget to remove when done!

#include "combat/Combat.h"
#include "combat/SequenceDice.h"
//#include "entities/Goblin.h"

//
#include "raylib.h"

#include <iostream>
#include <exception>

namespace
{
    dungeon::Action pollAction()
    {
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



    // TESTING! DON"T FORGET TO UPDATE WHEN DONE!
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
    if (argc < 2)
    {
        std::cerr
            << "Error: No configuration file was provided.\n"
            << "Usage: dungeon-crawler <config-file>\n";

        return 1;
    }

    dungeon::Game game;
    dungeon::Renderer renderer;

    try
    {
        game.load(argv[1]);
    }
    catch (const std::exception& error)
    {
        std::cerr
            << "Error: Failed to load configuration.\n"
            << error.what()
            << '\n';

        return 1;
    }

    // Run the diagnostic test right after loading the game configuration
    //runCombatDiagnostic(game);

    InitWindow(
        game.windowWidth(),
        game.windowHeight(),
        game.title().c_str());

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F1))
        {
            runCombatDiagnostic(game);
        }

        const dungeon::Action action = pollAction();

        game.handleAction(action);

        BeginDrawing();

        renderer.draw(game);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}