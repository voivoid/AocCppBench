#include <gtest/gtest.h>

#include "aoc/problems/2022_13.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem13)
{
    const char* const input1 = R"([1,1,3,1,1]
[1,1,5,1,1]

[[1],[2,3,4]]
[[1],4]

[9]
[[8,7,6]]

[[4,4],4,4]
[[4,4],4,4,4]

[7,7,7,7]
[7,7,7]

[]
[3]

[[[]]]
[[]]

[1,[2,[3,[4,[5,6,7]]]],8,9]
[1,[2,[3,[4,[5,6,0]]]],8,9]
)";

    EXPECT_EQ(solve(input1, &aoc::year_2022::solve13_a), 13);
    EXPECT_EQ(solve(input1, &aoc::year_2022::solve13_b), 140);
}