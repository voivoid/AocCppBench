#include <gtest/gtest.h>

#include "aoc/problems/2015_11.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem11)
{
    EXPECT_EQ(solve("abcdefgh", &aoc::year_2015::solve11_a), "abcdffaa");
}
