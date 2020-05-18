cmake_minimum_required(VERSION 3.0)
project(Compiler C)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif ()

set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_STANDARD 11)
include_directories(src)

set(SOURCE_FILES
        src/compiler_main.c
        src/vector.h
        src/command.h
        src/command.c
        src/compilation.h
        src/compilation.c)

add_executable(compiler ${SOURCE_FILES})