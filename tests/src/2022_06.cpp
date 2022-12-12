#include <gtest/gtest.h>

#include "aoc/problems/2022_06.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem06)
{
    EXPECT_EQ(solve("bvwbjplbgvbhsrlpgdmjqwftvncz", &aoc::year_2022::solve06_a), 5);
    EXPECT_EQ(solve("nppdvjthqldpwncqszvftbrmjlhg", &aoc::year_2022::solve06_a), 6);
    EXPECT_EQ(solve("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", &aoc::year_2022::solve06_a), 10);
    EXPECT_EQ(solve("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", &aoc::year_2022::solve06_a), 11);

    EXPECT_EQ(solve("mjqjpqmgbljsphdztnvjfqwrcgsmlb", &aoc::year_2022::solve06_b), 19);
    EXPECT_EQ(solve("bvwbjplbgvbhsrlpgdmjqwftvncz", &aoc::year_2022::solve06_b), 23);
    EXPECT_EQ(solve("nppdvjthqldpwncqszvftbrmjlhg", &aoc::year_2022::solve06_b), 23);
    EXPECT_EQ(solve("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", &aoc::year_2022::solve06_b), 29);
    EXPECT_EQ(solve("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", &aoc::year_2022::solve06_b), 26);
}