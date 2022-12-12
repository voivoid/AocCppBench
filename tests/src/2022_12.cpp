#include <gtest/gtest.h>

#include "aoc/problems/2022_12.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem12)
{
    const char* const input1 = R"(Sabqponm
abcryxxl
accszExk
acctuvwj
abdefghi
)";

    EXPECT_EQ(solve(input1, &aoc::year_2022::solve12_a), 31);
    EXPECT_EQ(solve(input1, &aoc::year_2022::solve12_b), 29);
}