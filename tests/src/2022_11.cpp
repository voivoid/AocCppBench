#include <gtest/gtest.h>

#include "aoc/problems/2022_11.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem11)
{
    const char* const input1 = R"(Monkey 0:
  Starting items: 79, 98
  Operation: new = old * 19
  Test: divisible by 23
    If true: throw to monkey 2
    If false: throw to monkey 3

Monkey 1:
  Starting items: 54, 65, 75, 74
  Operation: new = old + 6
  Test: divisible by 19
    If true: throw to monkey 2
    If false: throw to monkey 0

Monkey 2:
  Starting items: 79, 60, 97
  Operation: new = old * old
  Test: divisible by 13
    If true: throw to monkey 1
    If false: throw to monkey 3

Monkey 3:
  Starting items: 74
  Operation: new = old + 3
  Test: divisible by 17
    If true: throw to monkey 0
    If false: throw to monkey 1
)";

    EXPECT_EQ(solve(input1, &aoc::year_2022::solve11_a), 10605);
    EXPECT_EQ(solve(input1, &aoc::year_2022::solve11_b), 0 /*2713310158*/);
}