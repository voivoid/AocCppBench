#include <gtest/gtest.h>

#include "aoc/problems/2021_01.h"
#include "aoc/tests.h"

TEST(Aoc2021, problem01)
{
    const char* const input = R"(199
200
208
210
200
207
240
269
260
263)";

    EXPECT_EQ(solve(input, &aoc::year_2021::solve01_a), 7);
    EXPECT_EQ(solve(input, &aoc::year_2021::solve01_b), 5);
}
