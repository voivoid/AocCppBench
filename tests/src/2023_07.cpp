#include <gtest/gtest.h>

#include "aoc/problems/2023_07.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem07)
{
    const char* const input = R"(32T3K 765
T55J5 684
KK677 28
KTJJT 220
QQQJA 483)";

    EXPECT_EQ(solve(input, &aoc::year_2023::solve07_a), 6440);
    EXPECT_EQ(solve(input, &aoc::year_2023::solve07_b), 5905);
}
