set(SOURCES
    inc/aoc/problems/register.h
    inc/aoc/problems/all.h
    src/coords.h
    src/coords_fusion.h
    src/ensure.h
    src/fusion.h
    src/grid.h
    src/macros.h
    src/parse.h
    src/stream.h
    src/views.h

#2015
    inc/aoc/problems/2015_01.h src/2015_01.cpp
    inc/aoc/problems/2015_02.h src/2015_02.cpp
    inc/aoc/problems/2015_03.h src/2015_03.cpp
    inc/aoc/problems/2015_04.h src/2015_04.cpp
    inc/aoc/problems/2015_05.h src/2015_05.cpp
    inc/aoc/problems/2015_06.h src/2015_06.cpp
    inc/aoc/problems/2015_07.h src/2015_07.cpp
    inc/aoc/problems/2015_08.h src/2015_08.cpp
    inc/aoc/problems/2015_09.h src/2015_09.cpp
    inc/aoc/problems/2015_10.h src/2015_10.cpp
    inc/aoc/problems/2015_11.h src/2015_11.cpp
    inc/aoc/problems/2015_12.h src/2015_12.cpp
    inc/aoc/problems/2015_13.h src/2015_13.cpp
    inc/aoc/problems/2015_14.h src/2015_14.cpp

#2021
    inc/aoc/problems/2021_01.h src/2021_01.cpp
    inc/aoc/problems/2021_02.h src/2021_02.cpp
    inc/aoc/problems/2021_03.h src/2021_03.cpp
    inc/aoc/problems/2021_04.h src/2021_04.cpp
    inc/aoc/problems/2021_05.h src/2021_05.cpp
    inc/aoc/problems/2021_06.h src/2021_06.cpp
    inc/aoc/problems/2021_07.h src/2021_07.cpp

#2023
    inc/aoc/problems/2023_01.h src/2023_01.cpp
    inc/aoc/problems/2023_02.h src/2023_02.cpp
    inc/aoc/problems/2023_03.h src/2023_03.cpp
    inc/aoc/problems/2023_04.h src/2023_04.cpp
    inc/aoc/problems/2023_05.h src/2023_05.cpp
    inc/aoc/problems/2023_06.h src/2023_06.cpp
    inc/aoc/problems/2023_07.h src/2023_07.cpp
    inc/aoc/problems/2023_08.h src/2023_08.cpp
    inc/aoc/problems/2023_09.h src/2023_09.cpp
    inc/aoc/problems/2023_10.h src/2023_10.cpp
    inc/aoc/problems/2023_11.h src/2023_11.cpp
    inc/aoc/problems/2023_12.h src/2023_12.cpp
    inc/aoc/problems/2023_13.h src/2023_13.cpp
    inc/aoc/problems/2023_14.h src/2023_14.cpp
    inc/aoc/problems/2023_15.h src/2023_15.cpp
    inc/aoc/problems/2023_16.h src/2023_16.cpp
    inc/aoc/problems/2023_17.h src/2023_17.cpp
    inc/aoc/problems/2023_18.h src/2023_18.cpp
    inc/aoc/problems/2023_19.h src/2023_19.cpp
    inc/aoc/problems/2023_20.h src/2023_20.cpp
    inc/aoc/problems/2023_21.h src/2023_21.cpp
    inc/aoc/problems/2015_15.h src/2015_15.cpp
    inc/aoc/problems/2015_16.h src/2015_16.cpp
# ADD NEW PROBLEMS HERE
)

foreach(YEAR RANGE 2015 2023)
  source_group(${YEAR} ${SOURCES} REGULAR_EXPRESSION .+\/${YEAR}.+.cpp)
endforeach()

add_library(aoc_problems STATIC ${SOURCES})
add_library(aoc::problems ALIAS aoc_problems)
target_include_directories(aoc_problems PUBLIC inc PRIVATE src)
target_link_libraries(aoc_problems PRIVATE Boost::boost)
