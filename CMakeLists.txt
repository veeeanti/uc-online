cmake_minimum_required(VERSION 3.10)
project(uc-online VERSION 1.0.0 LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "This project is Windows-only")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)
include_directories(sdk/public)

# 32-bit version
add_executable(uc-online src/main.cpp src/uc_online.cpp src/ini_config.cpp src/logger.cpp src/resources.rc)
target_link_libraries(uc-online PRIVATE ${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/steam_api.lib kernel32)

# 64-bit version
add_executable(uc-online64 src/main64.cpp src/uc_online64.cpp src/ini_config.cpp src/logger.cpp src/resources.rc)
target_link_libraries(uc-online64 PRIVATE ${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/steam_api64.lib kernel32)
target_compile_definitions(uc-online64 PRIVATE IS_64BIT)

# Copy config.ini if it exists
if(EXISTS ${CMAKE_SOURCE_DIR}/config.ini)
    configure_file(config.ini config.ini COPYONLY)
endif()