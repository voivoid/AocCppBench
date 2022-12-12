#include <gtest/gtest.h>

#include "aoc/problems/2022_01.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem01)
{
    const char* const input = R"(1000
2000
3000

4000

5000
6000

7000
8000
9000

10000
)";

    EXPECT_EQ(solve(input, &aoc::year_2022::solve01_a), 24000);
    EXPECT_EQ(solve(input, &aoc::year_2022::solve01_b), 45000);
}