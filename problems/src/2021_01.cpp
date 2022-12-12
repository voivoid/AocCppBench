#include "aoc/problems/2021_01.h"

#include "ensure.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <vector>

namespace aoc
{
namespace year_2021
{

size_t solve01_functional_a(std::istream& input)
{
    const auto depths = std::ranges::istream_view<size_t>(input) | std::ranges::to<std::vector>();
    const auto result = std::ranges::count_if(depths | std::views::slide(2), [](const auto& slide) { return slide[ 1 ] > slide[ 0 ]; });

    return result;
}

size_t solve01_functional_b(std::istream& input)
{
    const auto depths = std::ranges::istream_view<size_t>(input) | std::ranges::to<std::vector>();
    const auto result = std::ranges::count_if(depths | std::views::slide(3) | std::views::slide(2),
                                              [](const auto& slide)
                                              {
                                                  const auto window1 = std::accumulate(slide[ 0 ].begin(), slide[ 0 ].end(), size_t(0));
                                                  const auto window2 = std::accumulate(slide[ 1 ].begin(), slide[ 1 ].end(), size_t(0));
                                                  return window2 > window1;
                                              });

    return result;
}

size_t solve01_a_imperative(std::istream& input)
{
    size_t depth_increases = 0;
    size_t prev_depth      = 0;
    size_t depth           = 0;

    ensure(input >> prev_depth);
    while (input >> depth)
    {
        if (depth > prev_depth)
            ++depth_increases;

        prev_depth = depth;
    }

    return depth_increases;
}

size_t solve01_b_imperative(std::istream& input)
{
    size_t depth_increases = 0;

    int d1, d2, d3, next_depth;

    ensure(input >> d1);
    ensure(input >> d2);
    ensure(input >> d3);
    while (input >> next_depth)
    {
        const int window      = d1 + d2 + d3;
        const int next_window = d2 + d3 + next_depth;

        if (next_window > window)
            ++depth_increases;

        d1 = d2;
        d2 = d3;
        d3 = next_depth;
    }

    return depth_increases;
}

}  // namespace year_2021
}  // namespace aoc