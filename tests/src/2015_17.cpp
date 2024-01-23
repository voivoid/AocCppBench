#include <gtest/gtest.h>

#include "aoc/problems/2015_17.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem17)
{
    const char* const input = R"(20
15
10
5
5)";

    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve17_a, size_t(25)), 4);
    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve17_b, size_t(25)), 3);
}
