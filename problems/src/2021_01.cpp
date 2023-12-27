#include "aoc/problems/2021_01.h"

#include "views.h"

#include <algorithm>
#include <istream>
#include <ranges>
#include <vector>

#include <boost/hana/functional/flip.hpp>

using namespace std::ranges;

namespace
{
template <size_t window_size>
size_t solve(std::istream& input)
{
    const auto depths = aoc::istream_buffered_view<int, window_size + 1>(input);

    const auto measures = depths | views::adjacent_transform<window_size>([](auto... ds) { return (... + ds); });

    return std::ranges::count(measures | views::pairwise_transform(boost::hana::flip(std::greater{})), true);
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
