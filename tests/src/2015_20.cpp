#include <gtest/gtest.h>

#include "aoc/problems/2015_20.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem20)
{
    const char* const input = R"()";

    EXPECT_EQ(solve(input, &aoc::year_2015::solve20_a), 0);
    EXPECT_EQ(solve(input, &aoc::year_2015::solve20_b), 0);
}
