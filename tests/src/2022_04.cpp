#include <gtest/gtest.h>

#include "aoc/problems/2022_04.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem04)
{
    const char* const input = R"(2-4,6-8
2-3,4-5
5-7,7-9
2-8,3-7
6-6,4-6
2-6,4-8
)";

    EXPECT_EQ(solve(input, &aoc::year_2022::solve04_a), 2);
    EXPECT_EQ(solve(input, &aoc::year_2022::solve04_b), 4);
}