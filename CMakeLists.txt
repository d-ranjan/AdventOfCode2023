cmake_minimum_required(VERSION 3.25)

project(AdventOfCode2023 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_GENERATOR "Ninja")
add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wunreachable-code)

list(APPEND CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-mingw-dynamic")

# add_subdirectory(day00)
# add_subdirectory(day01)
# add_subdirectory(day02)
# add_subdirectory(day03)
# add_subdirectory(day04)
# add_subdirectory(day05)
# add_subdirectory(day06)
# add_subdirectory(day07)
# add_subdirectory(day08)
# add_subdirectory(day09)
# add_subdirectory(day10) #TODO
# add_subdirectory(day11)
# add_subdirectory(day12) #TODO
# add_subdirectory(day15)
add_subdirectory(day16)
