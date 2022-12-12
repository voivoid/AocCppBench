#include <gtest/gtest.h>

#include "aoc/problems/2022_05.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem05)
{
    const char* const input = R"(    [D]    
[N] [C]    
[Z] [M] [P]
 1   2   3 

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2
)";

    EXPECT_EQ(solve(input, &aoc::year_2022::solve05_a), "CMZ");
    EXPECT_EQ(solve(input, &aoc::year_2022::solve05_b), "MCD");
}