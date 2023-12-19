#include <gtest/gtest.h>

#include "aoc/problems/2023_11.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem11)
{
    const char* const input = R"(...#......
.......#..
#.........
..........
......#...
.#........
.........#
..........
.......#..
#...#.....)";

    EXPECT_EQ(solve(input, &aoc::year_2023::solve11_a), 374);
    EXPECT_EQ(solve(input, &aoc::year_2023::solve11_b), 82000210);
}
