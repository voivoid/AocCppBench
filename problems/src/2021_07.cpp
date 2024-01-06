#include "aoc/problems/2021_07.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <ranges>

namespace
{
std::vector<int> parse_positions(std::istream& input)
{
    auto positions =
        aoc::x3_parse_attr<std::vector<int>>(input, boost::spirit::x3::int_ % ',', boost::spirit::x3::space);
    ensure(positions);

    return std::move(*positions);
}

size_t calc_constant_rate_fuel(const size_t moves)
{
    return moves;
}

size_t calc_increasing_rate_fuel(const size_t moves)
{
    return (moves * (moves + 1)) / 2;
}

template <auto calc_fuel>
size_t calc_total_fuel(const std::vector<int>& positions, const int best_align_pos)
{
    const auto fuel_costs =
        positions | std::ranges::views::transform([ best_align_pos ](const int pos)
                                                  { return calc_fuel(std::abs(best_align_pos - pos)); });

    const auto total_fuel = std::ranges::fold_left(fuel_costs, size_t(0), std::plus{});
    return total_fuel;
}

}  // namespace

namespace aoc
{
namespace year_2021
{

size_t solve07_a(std::istream& input)
{
    auto positions = parse_positions(input);

    int median = 0;
    if (positions.size() % 2 == 0)
    {
        auto right_median_iter = positions.begin() + (positions.size() / 2);
        auto left_median_iter  = right_median_iter - 1;

        std::ranges::nth_element(positions, left_median_iter);
        std::ranges::nth_element(positions, right_median_iter);

        median = (*left_median_iter + *right_median_iter) / 2;
    }
    else
    {
        auto median_iter = positions.begin() + (positions.size() / 2);
        std::ranges::nth_element(positions, median_iter);
        median = *median_iter;
    }

    return calc_total_fuel<calc_constant_rate_fuel>(positions, median);
}

size_t solve07_b(std::istream& input)
{
    const auto positions = parse_positions(input);
    const auto sum       = std::ranges::fold_left(positions, 0, std::plus{});
    const auto avg       = sum / static_cast<int>(positions.size());

    return std::min(calc_total_fuel<calc_increasing_rate_fuel>(positions, avg),
                    calc_total_fuel<calc_increasing_rate_fuel>(positions, avg + 1));
}

}  // namespace year_2021
}  // namespace aoc
