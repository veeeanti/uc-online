cmake_minimum_required(VERSION 3.10)
project(uc-online VERSION 1.0.0 LANGUAGES CXX)

# This is a Linux-only build
if(NOT UNIX)
    message(FATAL_ERROR "This project is Linux-only")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable all warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Include directories
include_directories(include)

# Function to create a build target for a specific architecture
function(add_linux_target target_name)
    add_executable(${target_name}
        src/main.cpp
        src/uc_online.cpp
        src/ini_config.cpp
        src/logger.cpp
    )
    
    # Set architecture-specific compile and link flags
    if(${target_name} STREQUAL "uc-online")
        set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
        add_compile_definitions(${target_name} PRIVATE IS_32BIT)
    else()
        add_compile_definitions(${target_name} PRIVATE IS_64BIT)
    endif()
    
    # Link libraries
    target_link_libraries(${target_name}
        PRIVATE
        dl
        pthread
    )
    
    # Set compile definitions for Linux
    target_compile_definitions(${target_name} PRIVATE LINUX=1 _LINUX=1)
    
    # Install target
    install(TARGETS ${target_name} DESTINATION bin)
endfunction()

# 32-bit version
add_linux_target(uc-online)

# 64-bit version
add_linux_target(uc-online64)

# Copy config.ini if it exists
if(EXISTS ${CMAKE_SOURCE_DIR}/config.ini)
    configure_file(config.ini config.ini COPYONLY)
endif()