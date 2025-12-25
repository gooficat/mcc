cmake_minimum_required(VERSION 3.15)
    project(as LANGUAGES C)
    
    # Set C standard
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    
    # Find vcpkg packages if available
    find_package(PkgConfig QUIET)
    
    # Include directories
    include_directories(include)
    
    # Add executable
    add_executable(as 
        src/main.c
    )
    
    # Compiler-specific options
    if(MSVC)
        target_compile_options(as PRIVATE /W4)
    else()
        target_compile_options(as PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    
    # Debug configuration
    set(CMAKE_BUILD_TYPE Debug)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(as PRIVATE DEBUG)
        if(NOT MSVC)
            target_compile_options(as PRIVATE -g -O0)
        endif()
    endif()
    
    # Enable testing
    enable_testing()
    
    # Install target
    #install(TARGETS as  RUNTIME DESTINATION bin)
        