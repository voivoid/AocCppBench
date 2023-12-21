#include <gtest/gtest.h>

#include "aoc/problems/2023_14.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem14)
{
    const char* const input = R"(O....#....
O.OO#....#
.....##...
OO.#O....O
.O.....O#.
O.#..O.#.#
..O..#O..O
.......O..
#....###..
#OO..#....)";

    EXPECT_EQ(solve(input, &aoc::year_2023::solve14_a), 136);
    EXPECT_EQ(solve(input, &aoc::year_2023::solve14_b), 64);
}
