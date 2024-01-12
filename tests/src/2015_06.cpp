#include <gtest/gtest.h>

#include "aoc/problems/2015_06.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem06)
{
    EXPECT_EQ(solve("toggle 0,0 through 999,0", &aoc::year_2015::solve06_a), 1000);
    EXPECT_EQ(solve("toggle 0,0 through 0,0", &aoc::year_2015::solve06_b), 2);
}
