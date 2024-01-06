#include <gtest/gtest.h>

#include "aoc/problems/2021_06.h"
#include "aoc/tests.h"

TEST(Aoc2021, problem06)
{
    const char* const input = R"(3,4,3,1,2)";

    EXPECT_EQ(solve(input, &aoc::year_2021::solve06_a), 5934);
    EXPECT_EQ(solve(input, &aoc::year_2021::solve06_b), 26984457539);
}
