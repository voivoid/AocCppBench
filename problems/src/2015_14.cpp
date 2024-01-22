#include "aoc/problems/2015_14.h"

#include "ensure.h"
#include "parse.h"

#include <boost/fusion/adapted/struct.hpp>

#include <algorithm>
#include <functional>
#include <ranges>

namespace
{
static constexpr size_t time_limit = 2503;
struct deer
{
    size_t speed;
    size_t fly_time;
    size_t rest_time;
};

struct state
{
    size_t distance;
    size_t score;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT(deer, speed, fly_time, rest_time)

namespace
{
auto make_parser()
{
    namespace x3      = boost::spirit::x3;
    const auto name   = x3::lexeme[ +x3::alpha ];
    const auto parser = x3::rule<struct _deer, deer>{} = x3::omit[ name ] > "can" > "fly" > aoc::x3_size_t_ > "km/s" >
                                                         "for" > aoc::x3_size_t_ > "seconds" > ',' > "but" > "then" >
                                                         "must" > "rest" > "for" > aoc::x3_size_t_ > "seconds" > '.';

    return parser;
}

size_t calc_distance(const deer& deer, const size_t seconds)
{
    const auto fly_rest_time   = deer.fly_time + deer.rest_time;
    const auto fly_rest_cycles = seconds / fly_rest_time;
    const auto fly_time_last   = std::min(deer.fly_time, seconds % fly_rest_time);
    const auto distance        = deer.speed * (deer.fly_time * fly_rest_cycles + fly_time_last);
    return distance;
}

}  // namespace

namespace aoc
{
namespace year_2015
{

namespace details
{
size_t solve14_a(std::istream& input, const size_t seconds)
{

    const auto distances = aoc::parse_lines<deer>(input, make_parser()) |
                           std::ranges::views::transform(std::bind_back(calc_distance, seconds));
    const auto max_distance = *std::max_element(distances.cbegin(), distances.cend());
    return max_distance;
}

size_t solve14_b(std::istream& input, const size_t seconds)
{
    const auto deer    = aoc::parse_lines<::deer>(input, make_parser()) | std::ranges::to<std::vector<::deer>>();
    const auto deer_sz = deer.size();

    std::vector<state> states(deer_sz, state{});
    std::vector<size_t> deer_to_score;
    deer_to_score.reserve(deer_sz);

    for (size_t sec = 0; sec < seconds; ++sec)
    {
        size_t max_distance = 0;
        for (size_t i = 0; i < deer_sz; ++i)
        {
            const auto& the_deer = deer[ i ];
            const bool is_flying = (sec % (the_deer.fly_time + the_deer.rest_time)) < the_deer.fly_time;
            if (is_flying) states[ i ].distance += the_deer.speed;

            if (states[ i ].distance > max_distance)
            {
                max_distance = states[ i ].distance;
                deer_to_score.clear();
            }

            if (states[ i ].distance == max_distance) { deer_to_score.push_back(i); }
        }

        for (const auto i : deer_to_score) { ++states[ i ].score; }

        deer_to_score.clear();
    }

    const auto winner = std::ranges::max_element(states, {}, &state::score);
    return winner->score;
}
}  // namespace details


size_t solve14_a(std::istream& input)
{
    return details::solve14_a(input, time_limit);
}

size_t solve14_b(std::istream& input)
{
    return details::solve14_b(input, time_limit);
}

}  // namespace year_2015
}  // namespace aoc
