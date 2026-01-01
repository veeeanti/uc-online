cmake_minimum_required(VERSION 3.10)
project(uc-online VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# 32-bit version
if(WIN32)
    add_executable(uc-online src/main.cpp src/uc_online.cpp src/ini_config.cpp src/logger.cpp src/resources.rc)
else()
    add_executable(uc-online src/main.cpp src/uc_online.cpp src/ini_config.cpp src/logger.cpp)
endif()

# Link libraries
if(WIN32)
    target_link_libraries(uc-online PRIVATE kernel32)
else()
    target_link_libraries(uc-online PRIVATE dl)
endif()

# 64-bit version
if(WIN32)
    add_executable(uc-online64 src/main64.cpp src/uc_online64.cpp src/ini_config.cpp src/logger.cpp src/resources.rc)
else()
    add_executable(uc-online64 src/main64.cpp src/uc_online64.cpp src/ini_config.cpp src/logger.cpp)
endif()

if(WIN32)
    target_link_libraries(uc-online64 PRIVATE kernel32)
    target_compile_definitions(uc-online64 PRIVATE IS_64BIT)
else()
    target_link_libraries(uc-online64 PRIVATE dl)
    target_compile_definitions(uc-online64 PRIVATE IS_64BIT)
endif()

# Copy config.ini if it exists
if(EXISTS ${CMAKE_SOURCE_DIR}/config.ini)
    configure_file(config.ini config.ini COPYONLY)
endif()