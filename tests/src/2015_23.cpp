#include <gtest/gtest.h>

#include "aoc/problems/2015_23.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem23)
{
    const char* const input = R"(inc a
inc b
tpl a
jie a, +2
inc b)";

    EXPECT_EQ(solve(input, &aoc::year_2015::solve23_a), 2);
    EXPECT_EQ(solve(input, &aoc::year_2015::solve23_b), 1);
}
