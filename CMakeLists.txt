cmake_minimum_required(VERSION 3.10)
project(uc-online VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# 32-bit DLL version
add_library(uc-online SHARED src/dllmain.cpp src/uc_online.cpp src/ini_config.cpp src/logger.cpp src/resources.rc)

# For Windows, link kernel32 for LoadLibrary, etc.
if(WIN32)
    target_link_libraries(uc-online PRIVATE kernel32 winmm)
    target_compile_definitions(uc-online PRIVATE IS_32BIT UC_ONLINE_EXPORTS)
    # Force 32-bit compilation
    if(MSVC)
        set_target_properties(uc-online PROPERTIES
            COMPILE_FLAGS "/arch:IA32"
            LINK_FLAGS "/MACHINE:X86"
        )
    endif()
endif()

# 64-bit DLL version
add_library(uc-online64 SHARED src/dllmain64.cpp src/uc_online64.cpp src/ini_config.cpp src/logger.cpp src/resources.rc)
if(WIN32)
    target_link_libraries(uc-online64 PRIVATE kernel32 winmm)
    target_compile_definitions(uc-online64 PRIVATE IS_64BIT UC_ONLINE_EXPORTS)
    # Force 64-bit compilation
    if(MSVC)
        set_target_properties(uc-online64 PROPERTIES
            LINK_FLAGS "/MACHINE:X64"
        )
    endif()
endif()

# Copy config.ini if it exists
if(EXISTS ${CMAKE_SOURCE_DIR}/config.ini)
    configure_file(config.ini config.ini COPYONLY)
endif()