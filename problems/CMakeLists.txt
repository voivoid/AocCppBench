set(SOURCES
    inc/aoc/problems/register.h
    inc/aoc/problems/all.h
    inc/aoc/problems/2021_01.h
    src/2021_01.cpp
    inc/aoc/problems/2021_02.h
    src/2021_02.cpp
    inc/aoc/problems/2021_03.h
    src/2021_03.cpp
    inc/aoc/problems/2021_04.h
    src/2021_04.cpp
    inc/aoc/problems/2022_01.h
    src/2022_01.cpp
    inc/aoc/problems/2022_02.h
    src/2022_02.cpp
    inc/aoc/problems/2022_03.h
    src/2022_03.cpp
    inc/aoc/problems/2022_04.h
    src/2022_04.cpp
    inc/aoc/problems/2022_05.h
    src/2022_05.cpp
    inc/aoc/problems/2022_06.h
    src/2022_06.cpp
    inc/aoc/problems/2022_07.h
    src/2022_07.cpp
    inc/aoc/problems/2022_08.h
    src/2022_08.cpp
    inc/aoc/problems/2022_09.h
    src/2022_09.cpp
    inc/aoc/problems/2022_10.h
    src/2022_10.cpp
    inc/aoc/problems/2022_11.h
    src/2022_11.cpp
    inc/aoc/problems/2022_12.h
    src/2022_12.cpp
    inc/aoc/problems/2022_13.h
    src/2022_13.cpp
    inc/aoc/problems/2022_14.h
    src/2022_14.cpp
    src/coord.h
    src/coord_fusion.h
    src/ensure.h
    src/parse.h
)

add_library(aoc_problems STATIC ${SOURCES})
add_library(aoc::problems ALIAS aoc_problems)
target_include_directories(aoc_problems PUBLIC inc PRIVATE src)
target_link_libraries(aoc_problems PRIVATE Boost::boost)