#include <gtest/gtest.h>

#include "aoc/problems/2023_13.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem13)
{
    const char* const input = R"(#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.

#...##..#
#....#..#
..##..###
#####.##.
#####.##.
..##..###
#....#..#)";

    EXPECT_EQ(solve(input, &aoc::year_2023::solve13_a), 405);
    EXPECT_EQ(solve(input, &aoc::year_2023::solve13_b), 400);
}
