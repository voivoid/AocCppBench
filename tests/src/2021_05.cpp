#include <gtest/gtest.h>

#include "aoc/problems/2021_05.h"
#include "aoc/tests.h"

TEST(Aoc2021, problem05)
{
    const char* const input = R"(0,9 -> 5,9
8,0 -> 0,8
9,4 -> 3,4
2,2 -> 2,1
7,0 -> 7,4
6,4 -> 2,0
0,9 -> 2,9
3,4 -> 1,4
0,0 -> 8,8
5,5 -> 8,2)";

    EXPECT_EQ(solve(input, &aoc::year_2021::solve05_a), 5);
    EXPECT_EQ(solve(input, &aoc::year_2021::solve05_b), 12);
}
