#include "aoc/problems/2023_06.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <cmath>
#include <ranges>
#include <tuple>
#include <vector>

namespace
{
struct parsed_input
{
    size_t time;
    size_t distance;
};

std::vector<parsed_input> parse_input_a(std::istream& input)
{
    namespace x3 = boost::spirit::x3;

    const auto time_parser     = x3::lit("Time:") > +aoc::x3_size_t_;
    const auto distance_parser = x3::lit("Distance:") > +aoc::x3_size_t_;
    const auto parser          = time_parser > distance_parser;

    auto attr = aoc::x3_parse_attr<std::tuple<std::vector<size_t>, std::vector<size_t>>>(input, parser, x3::space);
    ensure(attr);

    const auto& [ times, distances ] = *attr;

    auto inputs = std::ranges::views::zip(times, distances) | std::ranges::views::transform(
                                                                  [](const auto a)
                                                                  {
                                                                      auto [ t, d ] = a;
                                                                      return parsed_input(t, d);
                                                                  });

    return std::vector<parsed_input>(inputs.begin(), inputs.end());
}

std::vector<parsed_input> parse_input_b(std::istream& input)
{
    namespace x3 = boost::spirit::x3;

    const auto time_parser     = x3::lit("Time:") > +x3::digit;
    const auto distance_parser = x3::lit("Distance:") > +x3::digit;
    const auto parser          = time_parser > distance_parser;

    const auto attr = aoc::x3_parse_attr<std::tuple<std::string, std::string>>(input, parser, x3::space);
    ensure(attr);

    const auto& [ time, distance ] = *attr;

    return { parsed_input{ std::stoull(time), std::stoull(distance) } };
}

size_t calc_ways_to_beat_record(const parsed_input& input)
{
    const auto t = input.time;
    const auto d = input.distance;

    const auto D  = t * t - 4 * d;
    const auto x1 = (t - std::sqrt(static_cast<double>(D))) / (2.0) + 1;
    const auto x2 = (t + std::sqrt(static_cast<double>(D))) / (2.0) - 1;

    const size_t v1 = static_cast<size_t>(std::floor(x1));
    const size_t v2 = static_cast<size_t>(std::ceil(x2));
    assert(v2 > v1);

    const auto total = (v2 - v1) + 1;

    return total;
}

size_t solve(const std::vector<parsed_input>& input_data)
{
    const auto ways_to_beat_record = input_data | std::ranges::views::transform(&calc_ways_to_beat_record);
    const auto multiply_ways       = std::ranges::fold_left(ways_to_beat_record, size_t(1), std::multiplies{});

    return multiply_ways;
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve06_a(std::istream& input)
{
    return solve(parse_input_a(input));
}

size_t solve06_b(std::istream& input)
{
    return solve(parse_input_b(input));
}

}  // namespace year_2023
}  // namespace aoc