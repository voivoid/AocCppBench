#include <gtest/gtest.h>

#include "aoc/problems/2022_09.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem09)
{
    const char* const input1 = R"(R 4
U 4
L 3
D 1
R 4
D 1
L 5
R 2
)";

    EXPECT_EQ(solve(input1, &aoc::year_2022::solve09_a), 13);
    EXPECT_EQ(solve(input1, &aoc::year_2022::solve09_b), 1);

    const char* const input2 = R"(R 5
U 8
L 8
D 3
R 17
D 10
L 25
U 20
)";

    EXPECT_EQ(solve(input2, &aoc::year_2022::solve09_b), 36);
}