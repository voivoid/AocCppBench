#include <gtest/gtest.h>

#include "aoc/problems/2015_04.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem04)
{
    EXPECT_EQ(solve("abcdef", &aoc::year_2015::solve04_test), 298);
}
