#include <gtest/gtest.h>

#include "aoc/problems/2021_02.h"
#include "aoc/tests.h"

TEST(Aoc2021, problem02)
{
    const char* const input = R"(forward 5
down 5
forward 8
up 3
down 8
forward 2)";

    EXPECT_EQ(solve(input, &aoc::year_2021::solve02_a), 150);
    EXPECT_EQ(solve(input, &aoc::year_2021::solve02_b), 900);
}
