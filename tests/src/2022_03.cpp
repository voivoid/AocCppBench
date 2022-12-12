#include <gtest/gtest.h>

#include "aoc/problems/2022_03.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem03)
{
    const char* const input = R"(vJrwpWtwJgWrhcsFMMfFFhFp
jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
PmmdzqPrVvPwwTWBwg
wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
ttgJtRGJQctTZtZT
CrZsJsPPZsGzwwsLwLmpwMDw
)";

    EXPECT_EQ(solve(input, &aoc::year_2022::solve03_a), 157);
    EXPECT_EQ(solve(input, &aoc::year_2022::solve03_b), 70);
}