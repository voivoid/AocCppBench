#include <gtest/gtest.h>

#include "aoc/problems/2015_02.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem02)
{
    EXPECT_EQ(solve("2x3x4", &aoc::year_2015::solve02_a), 58);
    EXPECT_EQ(solve("1x1x10", &aoc::year_2015::solve02_a), 43);
    EXPECT_EQ(solve("2x3x4", &aoc::year_2015::solve02_b), 34);
    EXPECT_EQ(solve("1x1x10", &aoc::year_2015::solve02_b), 14);
}
