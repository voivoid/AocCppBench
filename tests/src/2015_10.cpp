#include <gtest/gtest.h>

#include "aoc/problems/2015_10.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem10)
{
    EXPECT_EQ(solve("1", &aoc::year_2015::details::solve10, size_t(10)), 26);
}
