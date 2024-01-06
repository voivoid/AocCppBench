set(SOURCES
    inc/aoc/tests.h
    src/2015_01.cpp
    src/2021_01.cpp
    src/2021_02.cpp
    src/2021_03.cpp
    src/2021_04.cpp
    src/2021_05.cpp
    src/2021_06.cpp
    src/2023_01.cpp
    src/2023_02.cpp
    src/2023_03.cpp
    src/2023_04.cpp
    src/2023_05.cpp
    src/2023_06.cpp
    src/2023_07.cpp
    src/2023_08.cpp
    src/2023_09.cpp
    src/2023_10.cpp
    src/2023_11.cpp
    src/2023_12.cpp
    src/2023_13.cpp
    src/2023_14.cpp
    src/2023_15.cpp
    src/2023_16.cpp
    src/2023_17.cpp
    src/2023_18.cpp
    src/2023_19.cpp
    src/2023_20.cpp
    src/2023_21.cpp
    src/2021_07.cpp
# ADD NEW TESTS HERE
)

foreach(YEAR RANGE 2015 2023)
  source_group(${YEAR} ${SOURCES} REGULAR_EXPRESSION .+\/${YEAR}.+)
endforeach()


add_executable(aoc_tests ${SOURCES})
target_include_directories(aoc_tests PRIVATE inc)
target_link_libraries(aoc_tests PRIVATE GTest::gtest_main aoc::problems)
add_test(NAME "AoC tests" COMMAND aoc_tests)


function(add_aoc_test problem result1 result2)
    set(App $<TARGET_FILE:aoc_solver>)

    add_test(NAME ${problem}_a
             COMMAND ${App} ${problem}_a )
    set_tests_properties(${problem}_a PROPERTIES PASS_REGULAR_EXPRESSION "^${result1}\n$")

    add_test(NAME ${problem}_b
             COMMAND ${App} ${problem}_b )
    set_tests_properties(${problem}_b PROPERTIES PASS_REGULAR_EXPRESSION "^${result2}\n$")
endfunction()

add_aoc_test(2015_01 280 1797)
add_aoc_test(2021_01 1766 1797)
add_aoc_test(2021_02 1728414 1765720035)
add_aoc_test(2021_03 2261546 6775520)
add_aoc_test(2021_04 28082 8224)
add_aoc_test(2021_05 6564 19172)
add_aoc_test(2021_06 346063 1572358335990)
add_aoc_test(2021_07 326132 88612508)
add_aoc_test(2023_01 54601 54078)
add_aoc_test(2023_02 2278 67953)
add_aoc_test(2023_03 559667 86841457)
add_aoc_test(2023_04 21105 5329815)
add_aoc_test(2023_05 993500720 4917124)
add_aoc_test(2023_06 840336 41382569)
add_aoc_test(2023_07 253866470 254494947)
add_aoc_test(2023_08 18157 14299763833181)
add_aoc_test(2023_09 1743490457 1053)
add_aoc_test(2023_10 6864 349)
add_aoc_test(2023_11 9565386 857986849428)
add_aoc_test(2023_12 8270 204640299929836)
add_aoc_test(2023_13 27742 32728)
add_aoc_test(2023_14 106186 106390)
add_aoc_test(2023_15 517551 286097)
add_aoc_test(2023_16 7060 7493)
add_aoc_test(2023_17 886 1055)
add_aoc_test(2023_18 42317 83605563360288)
add_aoc_test(2023_19 398527 133973513090020)
add_aoc_test(2023_20 684125385 225872806380073)
add_aoc_test(2023_21 3758 0)
