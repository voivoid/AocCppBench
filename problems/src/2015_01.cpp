#include "aoc/problems/2015_01.h"

#include <algorithm>
#include <istream>
#include <ranges>

namespace
{
auto get_moves(std::istream& input)
{
    return std::ranges::views::istream<char>(input) | std::ranges::views::transform(
                                                          [](const char c)
                                                          {
                                                              switch (c)
                                                              {
                                                                  case '(': return 1;
                                                                  case ')': return -1;
                                                              }

                                                              std::unreachable();
                                                          });
}
}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve01_a(std::istream& input)
{
    return std::ranges::fold_left(get_moves(input), size_t(0), std::plus{});
}

size_t solve01_b(std::istream& input)
{
    // TODO: use c++26 partial_sum view when available

    auto moves = get_moves(input);

    size_t steps = 0;
    int floor    = 0;
    for (const auto move : moves)
    {
        ++steps;
        floor += move;
        if (floor < 0) break;
    }

    return steps;
}

}  // namespace year_2015
}  // namespace aoc
