cmake_minimum_required(VERSION 3.10)
project(uc-online VERSION 1.0.0 LANGUAGES CXX)

# This is a Linux-only build
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "This project is strictly Linux-only.")
    message(STATUS "CMake configuration will continue, but compilation will fail on non-Linux systems.")
    message(STATUS "Current system: ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_PROCESSOR})")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable all warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Include directories
include_directories(include)
include_directories(sdk/public)

# Function to create a build target for a specific architecture
function(add_linux_target target_name arch_dir steam_lib)
    add_executable(${target_name}
        src/main.cpp
        src/uc_online.cpp
        src/ini_config.cpp
        src/logger.cpp
        src/error_dialog.cpp
    )
    
    # Set architecture-specific compile and link flags
    if(${arch_dir} STREQUAL "linux32")
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
        ${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/${arch_dir}/${steam_lib}
    )
    
    # Set compile definitions for Linux
    target_compile_definitions(${target_name} PRIVATE LINUX=1 _LINUX=1)
    
    # Install target
    install(TARGETS ${target_name} DESTINATION bin)
endfunction()

# 32-bit version
add_linux_target(uc-online linux32 libsteam_api.so)

# 64-bit version
add_linux_target(uc-online64 linux64 libsteam_api.so)

# Copy config.ini if it exists
if(EXISTS ${CMAKE_SOURCE_DIR}/config.ini)
    configure_file(config.ini config.ini COPYONLY)
endif()
