#include <gtest/gtest.h>

#include "aoc/problems/2015_09.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem09)
{
    const char* const input = R"(London to Dublin = 464
London to Belfast = 518
Dublin to Belfast = 141)";

    EXPECT_EQ(solve(input, &aoc::year_2015::solve09_a), 605);
    EXPECT_EQ(solve(input, &aoc::year_2015::solve09_b), 982);
}
