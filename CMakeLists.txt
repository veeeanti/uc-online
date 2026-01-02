cmake_minimum_required(VERSION 3.10)
project(uc-online VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dynamic linking for Steam API

# Include directories
include_directories(include)
include_directories(sdk/public)

# 32-bit version
add_executable(uc-online src/main.cpp src/uc_online.cpp src/ini_config.cpp src/logger.cpp)
set_target_properties(uc-online PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

# Link libraries
target_link_libraries(uc-online PRIVATE dl ${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/linux32/libsteam_api.so)

# 64-bit version
add_executable(uc-online64 src/main64.cpp src/uc_online64.cpp src/ini_config.cpp src/logger.cpp)

target_link_libraries(uc-online64 PRIVATE dl ${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/linux64/libsteam_api.so)
target_compile_definitions(uc-online64 PRIVATE IS_64BIT)

# Copy config.ini if it exists
if(EXISTS ${CMAKE_SOURCE_DIR}/config.ini)
    configure_file(config.ini config.ini COPYONLY)
endif()