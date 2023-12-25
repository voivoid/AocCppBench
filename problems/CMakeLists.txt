set(SOURCES
    inc/aoc/problems/register.h
    inc/aoc/problems/all.h
    inc/aoc/problems/2023_01.h src/2023_01.cpp
    inc/aoc/problems/2023_02.h src/2023_02.cpp
    inc/aoc/problems/2023_03.h src/2023_03.cpp
    inc/aoc/problems/2023_04.h src/2023_04.cpp
    inc/aoc/problems/2023_05.h src/2023_05.cpp
    inc/aoc/problems/2023_06.h src/2023_06.cpp
    inc/aoc/problems/2023_07.h src/2023_07.cpp
    inc/aoc/problems/2023_08.h src/2023_08.cpp
    inc/aoc/problems/2023_09.h src/2023_09.cpp
    inc/aoc/problems/2021_01.h src/2021_01.cpp
    inc/aoc/problems/2023_10.h src/2023_10.cpp
    inc/aoc/problems/2023_11.h src/2023_11.cpp
    inc/aoc/problems/2023_12.h src/2023_12.cpp
    inc/aoc/problems/2023_13.h src/2023_13.cpp
    inc/aoc/problems/2023_14.h src/2023_14.cpp
    inc/aoc/problems/2023_15.h src/2023_15.cpp
    inc/aoc/problems/2023_16.h src/2023_16.cpp
    inc/aoc/problems/2023_17.h src/2023_17.cpp
# ADD NEW PROBLEMS HERE
    src/coords.h
    src/coords_fusion.h
    src/ensure.h
    src/grid.h
    src/parse.h
)

add_library(aoc_problems STATIC ${SOURCES})
add_library(aoc::problems ALIAS aoc_problems)
target_include_directories(aoc_problems PUBLIC inc PRIVATE src)
target_link_libraries(aoc_problems PRIVATE Boost::boost)