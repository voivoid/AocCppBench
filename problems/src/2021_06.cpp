#include "aoc/problems/2021_06.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <array>
#include <functional>
#include <istream>
#include <ranges>

namespace
{
constexpr size_t birth_cycle = 9;

std::array<size_t, birth_cycle> parse_ages(std::istream& input)
{
    namespace views = std::ranges::views;
    namespace x3    = boost::spirit::x3;

    std::array<size_t, birth_cycle> ages{};
    for (const auto age : aoc::parse_lines<uint8_t, ','>(input, boost::spirit::x3::uint8)) { ++ages[ age ]; }

    return ages;
}

size_t solve(std::istream& input, size_t days)
{
    auto ages_indexed = parse_ages(input);
    while (days-- != 0)
    {
        std::rotate(ages_indexed.begin(), ages_indexed.begin() + 1, ages_indexed.end());
        ages_indexed[ 6 ] += ages_indexed[ 8 ];
    }

    const auto all_fish = std::ranges::fold_left(ages_indexed, size_t(0), std::plus{});
    return all_fish;
}

}  // namespace

namespace aoc
{
namespace year_2021
{

size_t solve06_a(std::istream& input)
{
    return solve(input, 80);
}

size_t solve06_b(std::istream& input)
{
    return solve(input, 256);
}

}  // namespace year_2021
}  // namespace aoc
