#include <gtest/gtest.h>

#include "aoc/problems/2015_15.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem15)
{
    const char* const input = R"(Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8
Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3)";

    EXPECT_EQ(solve(input, &aoc::year_2015::solve15_a), 62842880);
    EXPECT_EQ(solve(input, &aoc::year_2015::solve15_b), 57600000);
}
