#include <gtest/gtest.h>

#include "aoc/problems/2023_03.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem03)
{
    const char* const input = R"(467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..
)";

    EXPECT_EQ(solve(input, &aoc::year_2023::solve03_a), 4361);
    EXPECT_EQ(solve(input, &aoc::year_2023::solve03_b), 467835);
}
