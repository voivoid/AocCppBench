#include <gtest/gtest.h>

#include "aoc/problems/2023_12.h"
#include "aoc/tests.h"

TEST(Aoc2023, problem12)
{
    const char* const input = R"(???.### 1,1,3
.??..??...?##. 1,1,3
?#?#?#?#?#?#?#? 1,3,1,6
????.#...#... 4,1,1
????.######..#####. 1,6,5
?###???????? 3,2,1)";

    EXPECT_EQ(solve(input, &aoc::year_2023::solve12_a), 21);
    EXPECT_EQ(solve(input, &aoc::year_2023::solve12_b), 525152);
}
