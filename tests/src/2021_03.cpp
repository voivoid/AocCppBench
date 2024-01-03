#include <gtest/gtest.h>

#include "aoc/problems/2021_03.h"
#include "aoc/tests.h"

TEST(Aoc2021, problem03)
{
    const char* const input = R"(00100
11110
10110
10111
10101
01111
00111
11100
10000
11001
00010
01010)";

    EXPECT_EQ(solve(input, &aoc::year_2021::solve03_a), 198);
    EXPECT_EQ(solve(input, &aoc::year_2021::solve03_b), 230);
}
