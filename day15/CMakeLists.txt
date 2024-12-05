add_executable(day15
    input.h
    part1.h
    part2.h
    main.cpp
)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(day15 PRIVATE fmt::fmt)
