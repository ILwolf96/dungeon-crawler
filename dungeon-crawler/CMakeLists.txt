# CMakeList.txt : CMake project for dungeon-crawler, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

set(
    CMAKE_MSVC_RUNTIME_LIBRARY
    "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("dungeon-crawler")
add_subdirectory(config-parser)
# Add source to this project's executable.

#add_executable (dungeon-crawler "dungeon-crawler.cpp" "dungeon-crawler.h"  "config-parser/include/config/IConfigParser.h" "config-parser/include/config/ConfigData.h" "config-parser/include/config/IniParser.h" "config-parser/include/config/JsonParser.h" "config-parser/include/config/ParserFactory.h" "src/IniParser.cpp" "src/JsonParser.cpp" "src/ParserFactory.cpp")

add_executable(
    dungeon-crawler

    #"dungeon-crawler.cpp"
    #"dungeon-crawler.h"

    src/main.cpp
    "src/app/Game.cpp"
    "src/rendering/Renderer.cpp"
 "include/world/Map.h" "include/entities/Player.h" "src/world/Map.cpp" "src/entities/Player.cpp" "include/entities/Enemy.h" "src/entities/Enemy.cpp" "include/entities/Skeleton.h" "src/entities/Skeleton.cpp" "include/entities/Goblin.h" "src/entities/Goblin.cpp" "include/entities/Dragon.h" "src/entities/Dragon.cpp" "include/entities/EnemyFactory.h" "src/entities/EnemyFactory.cpp" "include/combat/CombatStats.h" "include/combat/CombatTarget.h" "include/entities/Orc.h" "include/entities/Troll.h" "src/entities/Orc.cpp" "src/entities/Troll.cpp" "include/combat/IDice.h" "include/combat/RandomDice.h" "src/combat/RandomDice.cpp" "include/combat/CombatRules.h" "src/combat/CombatRules.cpp" "include/gear/Equipment.h" "include/gear/Weapon.h" "src/gear/Weapon.cpp" "include/gear/Armor.h" "src/gear/Armor.cpp" "include/gear/Accessory.h" "src/gear/Accessory.cpp" "src/gear/Equipment.cpp" "include/gear/GearFactory.h" "src/gear/GearFactory.cpp" "include/inventory/Consumable.h" "include/inventory/RagePotion.h" "include/inventory/Inventory.h" "src/inventory/Inventory.cpp" "include/entities/Chest.h" "src/entities/Chest.cpp" "include/loot/LootReward.h" "include/loot/LootTable.h" "include/loot/LootGenerator.h" "src/loot/LootGenerator.cpp" "src/loot/LootTable.cpp")

 #set_target_properties(
 #  dungeon-crawler
 #   PROPERTIES
 #       VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
#)

add_custom_command(
    TARGET dungeon-crawler
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:dungeon-crawler>/data"
)

target_include_directories(
    dungeon-crawler
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


target_link_libraries(
    dungeon-crawler
    PRIVATE
        configparser
        raylib
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET dungeon-crawler PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
