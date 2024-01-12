#include <gtest/gtest.h>

#include "aoc/problems/2015_05.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem05)
{
    EXPECT_EQ(solve("ugknbfddgicrmopn", &aoc::year_2015::solve05_a), 1);
    EXPECT_EQ(solve("aaa", &aoc::year_2015::solve05_a), 1);
    EXPECT_EQ(solve("jchzalrnumimnmhp", &aoc::year_2015::solve05_a), 0);
    EXPECT_EQ(solve("haegwjzuvuyypxyu", &aoc::year_2015::solve05_a), 0);
    EXPECT_EQ(solve("dvszwmarrgswjxmb", &aoc::year_2015::solve05_a), 0);

    EXPECT_EQ(solve("qjhvhtzxzqqjkmpb", &aoc::year_2015::solve05_b), 1);
    EXPECT_EQ(solve("xxyxx", &aoc::year_2015::solve05_b), 1);
    EXPECT_EQ(solve("uurcxstgmygtbstg", &aoc::year_2015::solve05_b), 0);
    EXPECT_EQ(solve("ieodomkazucvgmuy", &aoc::year_2015::solve05_b), 0);
}
