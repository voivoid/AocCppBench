#include <gtest/gtest.h>

#include "aoc/problems/2015_08.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem08)
{
    const char* const input = R"(""
"abc"
"aaa\"aaa"
"\x27"
)";

    EXPECT_EQ(solve("\"\"", &aoc::year_2015::solve08_a), 2);
    EXPECT_EQ(solve(input, &aoc::year_2015::solve08_a), 12);

    EXPECT_EQ(solve("\"\"", &aoc::year_2015::solve08_b), 4);
    EXPECT_EQ(solve(input, &aoc::year_2015::solve08_b), 19);
}
