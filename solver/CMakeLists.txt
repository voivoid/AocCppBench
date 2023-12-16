add_executable(aoc_solver main.cpp)
target_compile_definitions(aoc_solver PRIVATE AOC_TEST_INPUT_DIR=\"${TEST_INPUT_DIR}\")
target_link_libraries(aoc_solver PRIVATE aoc::problems)