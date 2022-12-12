#include <gtest/gtest.h>

#include "aoc/problems/2022_07.h"
#include "aoc/tests.h"

TEST(Aoc2022, problem07)
{
    const char* const input = R"($ cd /
$ ls
dir a
14848514 b.txt
8504156 c.dat
dir d
$ cd a
$ ls
dir e
29116 f
2557 g
62596 h.lst
$ cd e
$ ls
584 i
$ cd ..
$ cd ..
$ cd d
$ ls
4060174 j
8033020 d.log
5626152 d.ext
7214296 k
)";

    EXPECT_EQ(solve(input, &aoc::year_2022::solve07_a), 95437);
    EXPECT_EQ(solve(input, &aoc::year_2022::solve07_b), 24933642);
}