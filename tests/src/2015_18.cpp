#include <gtest/gtest.h>

#include "aoc/problems/2015_18.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem18)
{
    const char* const input = R"(.#.#.#
...##.
#....#
..#...
#.#..#
####..)";

    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve18_a, size_t(4)), 4);
    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve18_b, size_t(5)), 17);
}
