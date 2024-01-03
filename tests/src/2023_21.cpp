#include <gtest/gtest.h>

#include "aoc/problems/2023_21.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem21)
{
    const char* const input = R"(...........
.....###.#.
.###.##..#.
..#.#...#..
....#.#....
.##..S####.
.##..#...#.
.......##..
.##.#.####.
.##..##.##.
...........)";

    EXPECT_EQ(solve(input, &aoc::year_2023::details::solve21_a_impl, size_t(6)), 16);
    /*EXPECT_EQ(solve(input, &aoc::year_2023::details::solve21_b_impl, size_t(10)), 50);
    EXPECT_EQ(solve(input, &aoc::year_2023::details::solve21_b_impl, size_t(50)), 1594);
    EXPECT_EQ(solve(input, &aoc::year_2023::details::solve21_b_impl, size_t(100)), 6536);
    EXPECT_EQ(solve(input, &aoc::year_2023::details::solve21_b_impl, size_t(500)), 167004);
    EXPECT_EQ(solve(input, &aoc::year_2023::details::solve21_b_impl, size_t(1000)), 668697);
    EXPECT_EQ(solve(input, &aoc::year_2023::details::solve21_b_impl, size_t(5000)), 16733044);*/
}
