#include "aoc/problems/2021_02.h"

#include "coords.h"
#include "parse.h"

#include <algorithm>
#include <istream>
#include <ranges>

#include <boost/fusion/adapted/struct.hpp>

namespace
{
struct command
{
    aoc::direction dir;
    int distance;
};
}  // namespace

BOOST_FUSION_ADAPT_STRUCT(command, dir, distance)

namespace
{

auto make_command_parser()
{
    namespace x3 = boost::spirit::x3;

    const x3::symbols<aoc::direction> dir_symbols{ { "up", aoc::direction::up },
                                                   { "down", aoc::direction::down },
                                                   { "forward", aoc::direction::forward } };

    const auto parser = x3::rule<struct _cmd, command>{} = dir_symbols > x3::int_;
    return parser;
}

struct submarine_state
{
    aoc::generic_point<int> pos;
    int aim;
};

submarine_state calc_next_state1(submarine_state state, const command& cmd)
{
    auto& pos = state.pos;

    switch (cmd.dir)
    {
        case aoc::direction::up: pos.y -= cmd.distance; break;
        case aoc::direction::down: pos.y += cmd.distance; break;
        case aoc::direction::forward: pos.x += cmd.distance; break;
    }

    return state;
}

submarine_state calc_next_state2(submarine_state state, const command& cmd)
{
    auto& pos = state.pos;
    auto& aim = state.aim;

    switch (cmd.dir)
    {
        case aoc::direction::up: aim -= cmd.distance; break;
        case aoc::direction::down: aim += cmd.distance; break;
        case aoc::direction::forward:
            pos.x += cmd.distance;
            pos.y += aim * cmd.distance;
            break;
    }

    return state;
}

template <auto calc_state>
int solve(std::istream& input)
{
    const auto commands = aoc::parse_lines<command>(input, make_command_parser());

    const submarine_state initial_state{ { 0, 0 }, 0 };
    const auto final_state = std::ranges::fold_left(commands, initial_state, calc_state);
    return final_state.pos.x * final_state.pos.y;
}
}  // namespace

namespace aoc
{
namespace year_2021
{

int solve02_a(std::istream& input)
{
    return solve<&calc_next_state1>(input);
}

int solve02_b(std::istream& input)
{
    return solve<&calc_next_state2>(input);
}

}  // namespace year_2021
}  // namespace aoc
