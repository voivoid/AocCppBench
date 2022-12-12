#include <gtest/gtest.h>

#include "aoc/problems/2022_08.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem08)
{
    const char* const input = R"(30373
25512
65332
33549
35390
)";

    EXPECT_EQ(solve(input, &aoc::year_2022::solve08_a), 21);
    EXPECT_EQ(solve(input, &aoc::year_2022::solve08_b), 8);
}