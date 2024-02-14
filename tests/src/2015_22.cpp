#include <gtest/gtest.h>

#include "aoc/problems/2015_22.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem22)
{
    const char* const input = R"(Hit Points: 13
Damage: 16)";

    EXPECT_EQ(solve(input, &aoc::year_2015::solve22_a), 212);
    EXPECT_EQ(solve(input, &aoc::year_2015::solve22_b), 226);
}
