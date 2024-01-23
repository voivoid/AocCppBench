#include <gtest/gtest.h>

#include "aoc/problems/2015_16.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem16)
{
    const char* const input = R"(Sue 1: perfumes: 1, samoyeds: 4
Sue 2: goldfish: 5, trees: 4
Sue 3: children: 3, pomeranians: 3
Sue 4: akitas: 0, cars: 5)";

    EXPECT_EQ(solve(input, &aoc::year_2015::solve16_a), 3);
}
