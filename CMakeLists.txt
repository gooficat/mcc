cmake_minimum_required(VERSION 3.15)
    project(mcc LANGUAGES C)
    
    # Set C standard
    set(CMAKE_C_STANDARD 23)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    
    # Find vcpkg packages if available
    find_package(PkgConfig QUIET)
    
    # Include directories
    include_directories(include)
    include_directories("${CMAKE_SOURCE_DIR}/useful_snippets/c")
    
    file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.c)

    # Add executable
    add_executable(mcc 
        ${SOURCES}
        
        useful_snippets/c/defs.c
        useful_snippets/c/fancc.c
        useful_snippets/c/file.c
        useful_snippets/c/str.c
    )
    
    # Compiler-specific options
    if(MSVC)
        target_compile_options(mcc PRIVATE /W4)
    else()
        target_compile_options(mcc PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    
    # Debug configuration
    set(CMAKE_BUILD_TYPE Debug)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(mcc PRIVATE DEBUG)
        if(NOT MSVC)
            target_compile_options(mcc PRIVATE -g -O0)
        endif()
    endif()
    
    # Enable testing
    enable_testing()
    
    # Install target
    #install(TARGETS mcc  RUNTIME DESTINATION bin)
        