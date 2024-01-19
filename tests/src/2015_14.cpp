#include <gtest/gtest.h>

#include "aoc/problems/2015_14.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem14)
{
    const char* const input = R"(Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.
Dancer can fly 16 km/s for 11 seconds, but then must rest for 162 seconds.)";

    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve14_a, size_t(1000)), 1120);
    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve14_b, size_t(1000)), 689);
}
