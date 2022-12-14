#include <gtest/gtest.h>

#include "aoc/problems/2022_14.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem14)
{
    const char* const input1 = R"(498,4 -> 498,6 -> 496,6
503,4 -> 502,4 -> 502,9 -> 494,9
)";

    EXPECT_EQ(solve(input1, &aoc::year_2022::solve14_a), 24);
    EXPECT_EQ(solve(input1, &aoc::year_2022::solve14_b), 93);
}