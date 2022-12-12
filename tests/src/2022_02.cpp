#include <gtest/gtest.h>

#include "aoc/problems/2022_02.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem02)
{
    const char* const input = "A Y\nB X\nC Z\n";

    EXPECT_EQ(solve(input, &aoc::year_2022::solve02_a), 15);
    EXPECT_EQ(solve(input, &aoc::year_2022::solve02_b), 12);
}