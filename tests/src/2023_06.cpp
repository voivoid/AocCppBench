#include <gtest/gtest.h>

#include "aoc/problems/2023_06.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem06)
{
    const char* const input = R"(Time:      7  15   30
Distance:  9  40  200)";

    EXPECT_EQ(solve(input, &aoc::year_2023::solve06_a), 288);
    EXPECT_EQ(solve(input, &aoc::year_2023::solve06_b), 71503);
}
