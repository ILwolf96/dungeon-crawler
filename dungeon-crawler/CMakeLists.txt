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
     
    # I'm aware that I do not need any of the headers there, as those don't compile, but I want to leave those there for now just to make sure everything is as it should be.

    #Core
     src/main.cpp
     "src/app/Game.cpp"
     
     "src/rendering/Renderer.cpp"
     
     "include/world/Map.h"
     "src/world/Map.cpp" 

     #Player
     "include/entities/Player.h" 
     "src/entities/Player.cpp"
     
     #Enemies:
     "include/entities/Enemy.h" 
     "src/entities/Enemy.cpp" 
     
     "include/entities/EnemyFactory.h" 
     "src/entities/EnemyFactory.cpp" 
     
     "include/combat/CombatStats.h" 
     "include/combat/CombatTarget.h" 
     
     "include/entities/Chest.h" 
     "src/entities/Chest.cpp" 
     
     "include/entities/Goblin.h" 
     "src/entities/Goblin.cpp" 

     "include/entities/Skeleton.h" 
     "src/entities/Skeleton.cpp" 
     
     "include/entities/Orc.h" 
     "src/entities/Orc.cpp" 
     
     "include/entities/Troll.h" 
     "src/entities/Troll.cpp" 
     
     "include/entities/Dragon.h" 
     "src/entities/Dragon.cpp" 

     #Combat/Logic
     "include/combat/IDice.h" 
     
     "include/combat/RandomDice.h" 
     "src/combat/RandomDice.cpp" 
     
     "include/combat/CombatRules.h" 
     "src/combat/CombatRules.cpp" 

     #Loot
     "include/gear/Equipment.h" 
     "include/inventory/Consumable.h" 
     
     "include/gear/Weapon.h" 
     "src/gear/Weapon.cpp" 
     
     "include/gear/Armor.h" 
     "src/gear/Armor.cpp" 
     
     "include/gear/Accessory.h" 
     "src/gear/Accessory.cpp" 
     
     "src/gear/Equipment.cpp" 
     
     "include/gear/GearFactory.h" 
     "src/gear/GearFactory.cpp" 
     
     "include/inventory/Inventory.h" 
     "src/inventory/Inventory.cpp" 
     
     "include/inventory/HealthPotion.h" 
     "src/inventory/HealthPotion.cpp"
     
     "src/inventory/RagePotion.cpp"
     "include/inventory/RagePotion.h" 

     "include/loot/LootReward.h" 
     
     "include/loot/LootTable.h" 
     "src/loot/LootTable.cpp"
     
     "include/loot/LootGenerator.h" 
     "src/loot/LootGenerator.cpp" 

     # NEED To Sort!
 "include/combat/CombatResult.h" "include/combat/Combat.h" "src/combat/Combat.cpp" "include/combat/SequenceDice.h" "src/combat/SequenceDice.cpp" "include/rendering/MainScreenAssets.h" "src/rendering/MainScreenAssets.cpp" "include/combat/CombatPresentation.h" "src/combat/CombatPresentation.cpp")




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

add_custom_command(
    TARGET dungeon-crawler
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:dungeon-crawler>/assets"
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
