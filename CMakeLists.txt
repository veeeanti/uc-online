cmake_minimum_required(VERSION 3.10)
project(uc-online VERSION 1.0.0 LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "This project is Windows-only")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Suppress warnings for unsafe functions
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Include directories
include_directories(include)
include_directories(sdk/public)

# 32-bit version
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    add_executable(uc-online src/main.cpp src/uc_online.cpp src/ini_config.cpp src/logger.cpp src/resources.rc)
    target_link_libraries(uc-online PRIVATE ${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/32/steam_api.lib kernel32)
    target_compile_definitions(uc-online PRIVATE IS_32BIT)
endif()

# 64-bit version
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_executable(uc-online64 src/main64.cpp src/uc_online64.cpp src/ini_config.cpp src/logger.cpp src/resources.rc)
    target_link_libraries(uc-online64 PRIVATE ${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/64/steam_api64.lib kernel32)
    target_compile_definitions(uc-online64 PRIVATE IS_64BIT)
endif()

# Copy config.ini if it exists
if(EXISTS ${CMAKE_SOURCE_DIR}/config.ini)
    configure_file(config.ini config.ini COPYONLY)
endif()