#include <gtest/gtest.h>

#include "aoc/problems/2021_07.h"
#include "aoc/tests.h"

TEST(Aoc2021, problem07)
{
    const char* const input = R"(16,1,2,0,4,2,7,1,2,14)";

    EXPECT_EQ(solve(input, &aoc::year_2021::solve07_a), 37);
    EXPECT_EQ(solve(input, &aoc::year_2021::solve07_b), 168);
}
