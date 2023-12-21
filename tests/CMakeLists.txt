set(SOURCES
    inc/aoc/tests.h
    src/2023_01.cpp
    src/2023_02.cpp
    src/2023_03.cpp
    src/2023_04.cpp
    src/2023_05.cpp
    src/2023_06.cpp
    src/2023_07.cpp
    src/2023_08.cpp
    src/2023_09.cpp
    src/2021_01.cpp
    src/2023_10.cpp
    src/2023_11.cpp
    src/2023_12.cpp
    src/2023_13.cpp
    src/2023_14.cpp
# ADD NEW TESTS HERE
)

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

add_aoc_test(2023_01 54601 54078)
add_aoc_test(2023_02 2278 67953)
add_aoc_test(2023_03 559667 86841457)
add_aoc_test(2023_04 21105 5329815)
add_aoc_test(2023_05 993500720 4917124)
add_aoc_test(2023_06 840336 41382569)
add_aoc_test(2023_07 253866470 254494947)
add_aoc_test(2023_08 18157 14299763833181)
add_aoc_test(2023_09 1743490457 1053)
add_aoc_test(2021_01 1766 1797)
add_aoc_test(2023_10 6864 349)
add_aoc_test(2023_11 9565386 857986849428)
add_aoc_test(2023_12 8270 204640299929836)
add_aoc_test(2023_13 27742 32728)
add_aoc_test(2023_14 106186 106390)
