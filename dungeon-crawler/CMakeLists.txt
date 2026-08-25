# CMakeList.txt : CMake project for dungeon-crawler, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("dungeon-crawler")

# Add source to this project's executable.
add_executable (dungeon-crawler "dungeon-crawler.cpp" "dungeon-crawler.h"  "config-parser/include/config/IConfigParser.h" "config-parser/include/config/ConfigData.h" "config-parser/include/config/IniParser.h" "config-parser/include/config/JsonParser.h" "config-parser/include/config/ParserFactory.h" "src/IniParser.cpp" "src/JsonParser.cpp" "src/ParserFactory.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET dungeon-crawler PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
