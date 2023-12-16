#include <gtest/gtest.h>

#include "aoc/problems/2023_09.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem09)
{
    const char* const input = R"(0 3 6 9 12 15
1 3 6 10 15 21
10 13 16 21 30 45)";

    EXPECT_EQ(solve(input, &aoc::year_2023::solve09_a), 114);
    EXPECT_EQ(solve(input, &aoc::year_2023::solve09_b), 2);
}
