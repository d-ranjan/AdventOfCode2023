cmake_minimum_required(VERSION 3.16)

project(advent_of_code LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(advent_of_code
    # d1_part1.cpp
    # d1_part2.cpp
    d2_part1.cpp
)

include(GNUInstallDirs)
install(TARGETS advent_of_code
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
