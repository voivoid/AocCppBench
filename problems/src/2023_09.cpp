#include "aoc/problems/2023_09.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <functional>
#include <istream>
#include <ranges>
#include <vector>

#include <boost/hana/functional/flip.hpp>

namespace
{
auto make_parser()
{
    return +(boost::spirit::x3::int_ - boost::spirit::x3::eol);
}

int calc_extrapolation(std::vector<int> ns, const bool reverse_input)
{
    if (reverse_input) { std::ranges::reverse(ns); }

    int sum = 0;

    auto active_rng = std::span(ns);
    while (!active_rng.empty() && (std::ranges::any_of(active_rng, [](const auto n) { return n != 0; })))
    {
        sum += ns.back();

        const auto diffs = active_rng | std::ranges::views::pairwise_transform(boost::hana::flip(std::minus{}));

        std::ranges::copy(diffs | std::ranges::views::reverse, ns.rbegin());

        // active_rng has one fewer value than the input sequence because at each step it considers
        // two numbers from the input
        active_rng = active_rng.subspan(1);
    }

    return sum;
}

size_t solve(std::istream& input, const bool reverse_input = false)
{
    const auto lines        = aoc::parse_lines<std::vector<int>>(input, make_parser());
    const auto extrapolated = lines | std::ranges::views::transform(std::bind_back(calc_extrapolation, reverse_input));
    return std::ranges::fold_left(extrapolated, 0, std::plus{});
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve09_a(std::istream& input)
{
    return solve(input);
}

size_t solve09_b(std::istream& input)
{
    return solve(input, true);
}

}  // namespace year_2023
}  // namespace aoc
