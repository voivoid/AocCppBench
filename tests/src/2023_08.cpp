#include <gtest/gtest.h>

#include "aoc/problems/2023_08.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem08)
{
    const char* const input1 = R"(RL

AAA = (BBB, CCC)
BBB = (DDD, EEE)
CCC = (ZZZ, GGG)
DDD = (DDD, DDD)
EEE = (EEE, EEE)
GGG = (GGG, GGG)
ZZZ = (ZZZ, ZZZ))";

    const char* const input2 = R"(LLR

AAA = (BBB, BBB)
BBB = (AAA, ZZZ)
ZZZ = (ZZZ, ZZZ)
)";

    const char* const input3 = R"(LR

11A = (11B, XXX)
11B = (XXX, 11Z)
11Z = (11B, XXX)
22A = (22B, XXX)
22B = (22C, 22C)
22C = (22Z, 22Z)
22Z = (22B, 22B)
XXX = (XXX, XXX))";

    EXPECT_EQ(solve(input1, &aoc::year_2023::solve08_a), 2);
    EXPECT_EQ(solve(input2, &aoc::year_2023::solve08_a), 6);
    EXPECT_EQ(solve(input3, &aoc::year_2023::solve08_b), 6);
}
