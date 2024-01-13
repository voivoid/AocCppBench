#include <gtest/gtest.h>

#include "aoc/problems/2015_07.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem07)
{
    const char* const input = R"(123 -> x
456 -> y
x AND y -> d
x OR y -> e
x LSHIFT 2 -> f
y RSHIFT 2 -> g
NOT x -> h
NOT y -> i)";

    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve07_a, "x"), 123);
    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve07_a, "y"), 456);
    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve07_a, "d"), 72);
    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve07_a, "e"), 507);
    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve07_a, "f"), 492);
    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve07_a, "g"), 114);
    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve07_a, "h"), 65412);
    EXPECT_EQ(solve(input, &aoc::year_2015::details::solve07_a, "i"), 65079);
}
