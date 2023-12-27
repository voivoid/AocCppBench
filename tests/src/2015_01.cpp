#include <gtest/gtest.h>

#include "aoc/problems/2015_01.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem01)
{
    EXPECT_EQ(solve("(())", &aoc::year_2015::solve01_a), 0);
    EXPECT_EQ(solve("()()", &aoc::year_2015::solve01_a), 0);
    EXPECT_EQ(solve("(((", &aoc::year_2015::solve01_a), 3);
    EXPECT_EQ(solve("(()(()(", &aoc::year_2015::solve01_a), 3);
    EXPECT_EQ(solve("))(((((", &aoc::year_2015::solve01_a), 3);
    EXPECT_EQ(solve("())", &aoc::year_2015::solve01_a), -1);
    EXPECT_EQ(solve("))(", &aoc::year_2015::solve01_a), -1);
    EXPECT_EQ(solve(")))", &aoc::year_2015::solve01_a), -3);
    EXPECT_EQ(solve(")())())", &aoc::year_2015::solve01_a), -3);

    EXPECT_EQ(solve(")", &aoc::year_2015::solve01_b), 1);
    EXPECT_EQ(solve("()())", &aoc::year_2015::solve01_b), 5);
}
