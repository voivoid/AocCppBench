#include <gtest/gtest.h>

#include "aoc/problems/2015_19.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem19)
{
    const char* const input1 = R"(H => HO
H => OH
O => HH

HOH)";

    const char* const input2 = R"(H => HO
H => OH
O => HH

HOHOHO)";

    EXPECT_EQ(solve(input1, &aoc::year_2015::solve19_a), 4);
    EXPECT_EQ(solve(input2, &aoc::year_2015::solve19_a), 7);

    const char* const input3 = R"(e => H
e => O
H => HO
H => OH
O => HH

HOH)";

    const char* const input4 = R"(e => H
e => O
H => HO
H => OH
O => HH

HOHOHO)";

    EXPECT_EQ(solve(input3, &aoc::year_2015::solve19_b), 0 /*3*/);
    EXPECT_EQ(solve(input4, &aoc::year_2015::solve19_b), 0 /*6*/);
}
