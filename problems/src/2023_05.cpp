#include "aoc/problems/2023_05.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <ranges>
#include <vector>

#include <boost/fusion/adapted/struct.hpp>

namespace
{
using seed_id = size_t;

struct seed_range
{
    size_t destination;
    size_t source;
    size_t length;
};

struct almanac
{
    std::vector<seed_id> seeds_ids;
    std::vector<std::vector<seed_range>> seed_ranges_map;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT(seed_range, source, destination, length)
BOOST_FUSION_ADAPT_STRUCT(almanac, seeds_ids, seed_ranges_map)

namespace
{
almanac parse_almanac(std::istream& input)
{
    namespace x3 = boost::spirit::x3;

    const auto seeds_parser = x3::lit("seeds:") > +aoc::x3_size_t_;
    const auto parse_range  = x3::rule<struct _range, seed_range>{} =
        aoc::x3_size_t_ > aoc::x3_size_t_ > aoc::x3_size_t_;
    const auto parse_ranges = x3::omit[ +x3::alpha ] > x3::lit("-to-") > x3::omit[ x3::lexeme[ +x3::alpha ] ] >
                              x3::lit("map:") > +parse_range;
    const auto parser = seeds_parser > *parse_ranges;

    auto result = aoc::x3_parse_attr<almanac>(input, parser, x3::space);
    ensure(result);

    return *result;
}

struct map_result
{
    seed_id id;
    size_t seed_ids_safe_to_skip;
};

map_result map_seed_id(const seed_id id, const std::vector<seed_range>& ranges)
{
    size_t seeds_to_a_next_range = std::numeric_limits<size_t>::max();
    for (const auto& range : ranges)
    {
        if (id >= range.destination && id < range.destination + range.length)
        {
            return { id - range.destination + range.source, range.length - (id - range.destination) };
        }
        else if (id < range.destination)
        {
            seeds_to_a_next_range = std::min(seeds_to_a_next_range, range.destination - id);
        }
    }

    return { id, seeds_to_a_next_range };
}

map_result map_seed_id_to_location(seed_id id, const almanac& almanac)
{
    size_t seed_ids_safe_to_skip = std::numeric_limits<size_t>::max();

    for (const auto& ranges : almanac.seed_ranges_map)
    {
        const auto map_result = map_seed_id(id, ranges);

        id                    = map_result.id;
        seed_ids_safe_to_skip = std::min(seed_ids_safe_to_skip, map_result.seed_ids_safe_to_skip);
    }

    return { id, seed_ids_safe_to_skip };
}

template <typename R>
size_t solve(const almanac& almanac, const R& seeds_range)
{
    size_t min_location = std::numeric_limits<size_t>::max();
    for (auto [ from, len ] : seeds_range)
    {
        for (size_t seed_id = from, to = from + len; seed_id < to;)
        {
            const auto [ location, seed_ids_safe_to_skip ] = map_seed_id_to_location(seed_id, almanac);

            min_location = std::min(min_location, location);
            seed_id += seed_ids_safe_to_skip;
        }
    }

    return min_location;
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve05_a(std::istream& input)
{
    const auto almanac = parse_almanac(input);

    auto seeds_range = almanac.seeds_ids |
                       std::ranges::views::transform([](const auto seed_id) { return std::make_pair(seed_id, 1); });

    return solve(almanac, seeds_range);
}

size_t solve05_b(std::istream& input)
{
    const auto almanac = parse_almanac(input);

    auto seeds_range = almanac.seeds_ids | std::ranges::views::chunk(2) |
                       std::ranges::views::transform(
                           [](const auto seed_id_and_len)
                           {
                               const auto seed_id = seed_id_and_len[ 0 ];
                               const auto len     = seed_id_and_len[ 1 ];
                               return std::make_pair(seed_id, len);
                           });

    return solve(almanac, seeds_range);
}

}  // namespace year_2023
}  // namespace aoc