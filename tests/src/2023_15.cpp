#include <gtest/gtest.h>

#include "aoc/problems/2023_15.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem15)
{
    const char* const input = R"(rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7)";

    EXPECT_EQ(solve(input, &aoc::year_2023::solve15_a), 1320);
    EXPECT_EQ(solve(input, &aoc::year_2023::solve15_b), 145);
}
