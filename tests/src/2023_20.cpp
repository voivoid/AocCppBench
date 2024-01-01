#include <gtest/gtest.h>

#include "aoc/problems/2023_20.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem20)
{
    const char* const input1 = R"(broadcaster -> a, b, c
%a -> b
%b -> c
%c -> inv
&inv -> a)";

    const char* const input2 = R"(broadcaster -> a
%a -> inv, con
&inv -> b
%b -> con
&con -> output)";

    EXPECT_EQ(solve(input1, &aoc::year_2023::solve20_a), 32000000);
    EXPECT_EQ(solve(input2, &aoc::year_2023::solve20_a), 11687500);
}
