cmake_minimum_required(VERSION 3.15)

project(config-parser LANGUAGES CXX)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(nlohmann_json)

add_library(configparser STATIC
    src/ConfigData.cpp
    src/IniParser.cpp
    src/JsonParser.cpp
    src/ParserFactory.cpp
 "include/config/DataRepository.h")

target_include_directories(configparser
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(configparser PUBLIC cxx_std_20)

target_link_libraries(configparser
    PUBLIC
        nlohmann_json::nlohmann_json
)