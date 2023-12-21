#include "aoc/problems/2023_08.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <istream>
#include <ranges>
#include <string>
#include <unordered_map>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/integer/common_factor_rt.hpp>
#include <boost/static_string.hpp>

namespace
{
using node      = boost::static_string<3>;
using nodes_map = std::unordered_map<node, std::pair<node, node>>;

nodes_map parse_nodes_map(std::istream& input)
{
    namespace x3 = boost::spirit::x3;

    const auto node_parser = x3::repeat(3)[ x3::char_ ];

    const auto source_parser      = node_parser;
    const auto destination_parser = x3::rule<struct _dest, std::pair<node, node>>{} =
        x3::lit('(') > node_parser > x3::lit(',') > node_parser > x3::lit(')');
    const auto parser = *(source_parser > x3::lit('=') > destination_parser);

    auto attr = aoc::x3_parse_attr<nodes_map>(input, parser, x3::space);
    ensure(attr);

    return std::move(*attr);
}

size_t count_steps(const node& start, const std::string& turns, const nodes_map& nodes_map)
{
    node current_pos = start;

    size_t steps = 0;

    while (!current_pos.ends_with("Z"))
    {
        const size_t turn_idx = steps % turns.size();
        const char turn       = turns[ turn_idx ];

        assert(turn == 'L' || turn == 'R');
        const auto dir = (turn == 'L' ? &std::pair<node, node>::first : &std::pair<node, node>::second);

        const auto destination = nodes_map.find(current_pos)->second;
        current_pos            = destination.*dir;

        ++steps;
    }

    return steps;
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve08_a(std::istream& input)
{
    std::string turns;
    std::getline(input, turns);

    const auto nodes_map = parse_nodes_map(input);
    return count_steps("AAA", turns, nodes_map);
}

size_t solve08_b(std::istream& input)
{
    std::string turns;
    std::getline(input, turns);

    const auto nodes_map = parse_nodes_map(input);

    auto cycles =
        nodes_map | std::ranges::views::keys |
        std::ranges::views::filter([](const node& n) { return n.ends_with('A'); }) |
        std::ranges::views::transform([ & ](const node& start) { return count_steps(start, turns, nodes_map); });

    const auto total_steps = boost::integer::lcm_range(cycles.cbegin(), cycles.cend()).first;

    return total_steps;
}

}  // namespace year_2023
}  // namespace aoc
