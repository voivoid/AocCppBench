#include <gtest/gtest.h>

#include "aoc/problems/2015_21.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem21)
{
    const char* const input = R"(Hit Points: 104
Damage: 8
Armor: 1)";

    EXPECT_EQ(solve(input, &aoc::year_2015::solve21_a), 78);
    EXPECT_EQ(solve(input, &aoc::year_2015::solve21_b), 148);
}
