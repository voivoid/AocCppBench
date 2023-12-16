#include "aoc/problems/2021_01.h"

#include <algorithm>
#include <istream>
#include <ranges>
#include <vector>

using namespace std::ranges;

namespace
{
template <size_t window_size>
size_t solve(std::istream& input)
{
    const auto depths = istream_view<int>(input) | to<std::vector<int>>();

    const auto measures = depths | views::slide(window_size) |
                          views::transform([](const auto triple) { return fold_left(triple, 0, std::plus{}); });

    return std::ranges::count_if(measures | views::slide(2),
                                 [](const auto measure_pair) { return measure_pair[ 1 ] > measure_pair[ 0 ]; });
}
}  // namespace

namespace aoc
{
namespace year_2021
{

size_t solve01_a(std::istream& input)
{
    return solve<1>(input);
}

size_t solve01_b(std::istream& input)
{
    return solve<3>(input);
}

}  // namespace year_2021
}  // namespace aoc
