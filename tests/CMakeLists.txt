set(SOURCES
    src/2021_01.cpp
    src/2021_02.cpp
    src/2021_03.cpp
    src/2021_04.cpp
    src/2022_01.cpp
    src/2022_02.cpp
    src/2022_03.cpp
    src/2022_04.cpp
    src/2022_05.cpp
    src/2022_06.cpp
    src/2022_07.cpp
    src/2022_08.cpp
    src/2022_09.cpp
    src/2022_10.cpp
    src/2022_11.cpp
    src/2022_12.cpp)

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

add_aoc_test(2021_01 1766 1797)
add_aoc_test(2021_01_functional 1766 1797)
add_aoc_test(2021_02 1728414 1765720035)
add_aoc_test(2021_03 2261546 6775520)
add_aoc_test(2021_04 28082 8224)

add_aoc_test(2022_01 69281 201524)
add_aoc_test(2022_02 10816 11657)
add_aoc_test(2022_03 7821 2752)
add_aoc_test(2022_04 511 821)
add_aoc_test(2022_05 TLFGBZHCN QRQFHFWCL)
add_aoc_test(2022_06 1802 3551)
add_aoc_test(2022_07 1315285 9847279)
add_aoc_test(2022_08 1713 268464)
add_aoc_test(2022_09 6464 2604)
add_aoc_test(2022_10 12520 "####\\.#\\.\\.#\\.###\\.\\.####\\.###\\.\\.\\.\\.##\\.\\.##\\.\\.#\\.\\.\\.\\..*")
add_aoc_test(2022_12 408 399)
