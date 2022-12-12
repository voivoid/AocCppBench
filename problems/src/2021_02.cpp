#include "aoc/problems/2021_02.h"

#include "parse.h"

#include <cassert>
#include <iostream>
#include <string>

#include "boost/fusion/adapted/struct.hpp"

namespace
{

struct state
{
    size_t x = 0;
    size_t y = 0;
    int aim  = 0;
};

enum class direction
{
    forward,
    down,
    up
};

struct command
{
    direction dir;
    size_t distance;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT(command, dir, distance)

namespace
{


auto make_parser()
{
    boost::spirit::x3::symbols<direction> direction;
    direction.add("forward", direction::forward)("down", direction::down)("up", direction::up);

    return direction > aoc::x3_size_t_;
}

void handle_cmd_v1(const command& cmd, state& state)
{
    switch (cmd.dir)
    {
        case direction::forward: state.x += cmd.distance; break;
        case direction::down: state.y += cmd.distance; break;
        case direction::up: state.y -= cmd.distance; break;
    }
}

void handle_cmd_v2(const command& cmd, state& state)
{
    switch (cmd.dir)
    {
        case direction::forward:
            state.x += cmd.distance;
            state.y += state.aim * cmd.distance;
            break;
        case direction::down: state.aim += static_cast<int>(cmd.distance); break;
        case direction::up: state.aim -= static_cast<int>(cmd.distance); break;
    }
}

template <void (*handle_cmd)(const command&, state&)>
size_t solve(std::istream& input)
{
    state state{};

    aoc::parse_each_line_and_exec(input, make_parser(), [ &state ](const command& cmd) { handle_cmd(cmd, state); });

    return state.x * state.y;
}

}  // namespace

namespace aoc
{
namespace year_2021
{

size_t solve02_a(std::istream& input)
{
    return solve<handle_cmd_v1>(input);
}

size_t solve02_b(std::istream& input)
{
    return solve<handle_cmd_v2>(input);
}

}  // namespace year_2021
}  // namespace aoc