#include "aoc/problems/2015_09.h"

#include "parse.h"

#include <boost/fusion/adapted/struct.hpp>

#include <algorithm>
#include <execution>
#include <functional>
#include <istream>
#include <map>
#include <ranges>
#include <string>
#include <tuple>

namespace
{

using location = std::string;
using distance = size_t;

struct route
{
    location from;
    location to;
    distance distance;
};

using routes_map = std::map<std::tuple<location, location>, distance, std::less<>>;
}  // namespace

BOOST_FUSION_ADAPT_STRUCT(route, from, to, distance)

namespace
{
auto make_parser()
{
    namespace x3 = boost::spirit::x3;

    const auto location = x3::lexeme[ +x3::alpha ];
    const auto parser   = location > x3::lit("to") > location > x3::lit('=') > aoc::x3_size_t_;

    return parser;
}

routes_map parse_map(std::istream& input)
{
    routes_map map;
    for (auto& route : aoc::parse_lines<route>(input, make_parser()))
    {
        map.emplace(std::make_tuple(route.from, route.to), route.distance);
        map.emplace(std::make_tuple(std::move(route.to), std::move(route.from)), route.distance);
    }

    return map;
}

distance calc_distance(const std::vector<location>& path, const routes_map& map)
{
    const auto route_distances = path | std::ranges::views::pairwise_transform(
                                            [ & ](const std::string& from, const std::string& to) {
                                                return map.find(std::make_tuple(std::ref(from), std::ref(to)))->second;
                                            });

    distance total_distance = 0;
    for (const auto distance : route_distances) { total_distance += distance; }

    return total_distance;
}

template <const distance& (*get_better_distance)(const distance&, const distance&)>
distance calc_best_distance(std::vector<location>& locations, const routes_map& map)
{
    std::sort(locations.begin() + 1, locations.end());

    distance best_distance = calc_distance(locations, map);
    while (std::next_permutation(locations.begin() + 1, locations.end()))
    {
        best_distance = get_better_distance(best_distance, calc_distance(locations, map));
    }

    return best_distance;
}

template <const distance& (*get_better_distance)(const distance&, const distance&)>
size_t solve(std::istream& input, const distance initial)
{
    const auto routes_map = parse_map(input);
    auto locations        = routes_map | std::ranges::views::keys |
                     std::ranges::views::transform([](const auto& pair) { return std::get<0>(pair); }) |
                     std::ranges::to<std::vector<location>>();

    std::ranges::sort(locations);
    locations.erase(std::unique(locations.begin(), locations.end()), locations.end());

    std::vector<std::vector<location>> locations_vec;
    for (size_t i = 0; i < locations.size() - 1; ++i)
    {
        locations_vec.push_back(locations);
        std::ranges::rotate(locations, locations.begin() + 1);
    }
    locations_vec.push_back(std::move(locations));

    size_t best_distance =
        std::transform_reduce(std::execution::par_unseq,
                              locations_vec.begin(),
                              locations_vec.end(),
                              initial,
                              get_better_distance,
                              std::bind_back(calc_best_distance<get_better_distance>, std::ref(routes_map)));
    return best_distance;
}
}  // namespace



namespace aoc
{
namespace year_2015
{

size_t solve09_a(std::istream& input)
{
    return solve<std::min>(input, distance(-1));
}

size_t solve09_b(std::istream& input)
{
    return solve<std::max>(input, 0);
}

}  // namespace year_2015
}  // namespace aoc
