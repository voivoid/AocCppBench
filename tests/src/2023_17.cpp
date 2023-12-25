#include <gtest/gtest.h>

#include "aoc/problems/2023_17.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem17)
{
    const char* const input1 = R"(2413432311323
3215453535623
3255245654254
3446585845452
4546657867536
1438598798454
4457876987766
3637877979653
4654967986887
4564679986453
1224686865563
2546548887735
4322674655533)";

    const char* const input2 = R"(111111111111
999999999991
999999999991
999999999991
999999999991)";

    EXPECT_EQ(solve(input1, &aoc::year_2023::solve17_a), 102);
    EXPECT_EQ(solve(input1, &aoc::year_2023::solve17_b), 94);
    EXPECT_EQ(solve(input2, &aoc::year_2023::solve17_b), 71);
}
