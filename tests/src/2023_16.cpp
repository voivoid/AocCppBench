#include <gtest/gtest.h>

#include "aoc/problems/2023_16.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem16)
{
    const char* const input = R"(.|...\....
|.-.\.....
.....|-...
........|.
..........
.........\
..../.\\..
.-.-/..|..
.|....-|.\
..//.|....)";

    EXPECT_EQ(solve(input, &aoc::year_2023::solve16_a), 46);
    EXPECT_EQ(solve(input, &aoc::year_2023::solve16_b), 51);
}
