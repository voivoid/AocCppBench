#include <gtest/gtest.h>

#include "aoc/problems/2015_12.h"
#include "aoc/tests.h"

TEST(Aoc2015, problem12)
{
    EXPECT_EQ(solve("[1,2,3]", &aoc::year_2015::solve12_a), 6);
    EXPECT_EQ(solve("{\"a\":2,\"b\":4}", &aoc::year_2015::solve12_a), 6);
    EXPECT_EQ(solve("[[[3]]]", &aoc::year_2015::solve12_a), 3);
    EXPECT_EQ(solve("{\"a\":{\"b\":4},\"c\":-1}", &aoc::year_2015::solve12_a), 3);
    EXPECT_EQ(solve("{\"a\":[-1,1]}", &aoc::year_2015::solve12_a), 0);
    EXPECT_EQ(solve("[-1,{\"a\":1}]", &aoc::year_2015::solve12_a), 0);
    EXPECT_EQ(solve("[]", &aoc::year_2015::solve12_a), 0);
    EXPECT_EQ(solve("{}", &aoc::year_2015::solve12_a), 0);

    EXPECT_EQ(solve("[1,2,3]", &aoc::year_2015::solve12_b), 6);
    EXPECT_EQ(solve("[1,{\"c\":\"red\",\"b\":2},3]", &aoc::year_2015::solve12_b), 4);
    EXPECT_EQ(solve("{\"d\":\"red\",\"e\":[1,2,3,4],\"f\":5}", &aoc::year_2015::solve12_b), 0);
    EXPECT_EQ(solve("[1,\"red\",5]", &aoc::year_2015::solve12_b), 6);
}
