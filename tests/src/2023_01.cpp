#include <gtest/gtest.h>

#include "aoc/problems/2023_01.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem01)
{
    const char* const input1 = R"(1abc2
pqr3stu8vwx
a1b2c3d4e5f
treb7uchet)";

    const char* const input2 = R"(two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen
)";

    EXPECT_EQ(solve(input1, &aoc::year_2023::solve01_a), 142);
    EXPECT_EQ(solve(input2, &aoc::year_2023::solve01_b), 281);
}
