cmake_minimum_required(VERSION 3.10)
project(UCOnline)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/uc_online.cpp
    src/ini_config.cpp
    src/logger.cpp
)

# Add executable
add_executable(uc_online ${SOURCES})

# Link libraries
target_link_libraries(uc_online
    dl
    pthread
)

# Enable warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(uc_online PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install target
install(TARGETS uc_online DESTINATION bin)