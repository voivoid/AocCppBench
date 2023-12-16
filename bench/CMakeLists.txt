add_executable(aoc_bench main.cpp)
target_compile_definitions(aoc_bench PRIVATE AOC_TEST_INPUT_DIR=\"${TEST_INPUT_DIR}\")
target_link_libraries(aoc_bench PRIVATE aoc::problems benchmark::benchmark)