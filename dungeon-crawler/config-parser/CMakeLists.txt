cmake_minimum_required(VERSION 3.15)

project(config-parser LANGUAGES CXX)

add_library(configparser STATIC
    src/IniParser.cpp
    src/JsonParser.cpp
    src/ParserFactory.cpp
)

target_include_directories(configparser
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(configparser PUBLIC cxx_std_20)