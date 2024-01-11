#include <gtest/gtest.h>

#include "aoc/problems/2015_03.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem03)
{
    const char* const input1 = "^>v<";
    const char* const input2 = "^v^v^v^v^v";

    EXPECT_EQ(solve("^", &aoc::year_2015::solve03_a), 2);
    EXPECT_EQ(solve(input1, &aoc::year_2015::solve03_a), 4);
    EXPECT_EQ(solve(input2, &aoc::year_2015::solve03_a), 2);

    EXPECT_EQ(solve("^v", &aoc::year_2015::solve03_b), 3);
    EXPECT_EQ(solve(input1, &aoc::year_2015::solve03_b), 3);
    EXPECT_EQ(solve(input2, &aoc::year_2015::solve03_b), 11);
}
