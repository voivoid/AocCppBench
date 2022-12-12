#include "aoc/problems/2022_10.h"

#include "ensure.h"
#include "parse.h"

#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <variant>

#include "boost/algorithm/string/join.hpp"

namespace
{
struct state
{
    int regX = 1;
};

namespace commands
{

struct noop
{
    static constexpr size_t ticks_to_complete = 1;
    void apply_changes(state& /*state*/) const
    {
    }
};

struct addx
{
    static constexpr size_t ticks_to_complete = 2;
    void apply_changes(state& state) const
    {
        state.regX += val;
    }

    int val;
};

}  // namespace commands

using command = std::variant<commands::noop, commands::addx>;

size_t get_ticks_to_complete(const command& cmd)
{
    return std::visit([]<typename T>(const T&) { return T::ticks_to_complete; }, cmd);
}

void finish_execution(state& state, const command& cmd)
{
    std::visit([ &state ](const auto& dispatched_cmd) { dispatched_cmd.apply_changes(state); }, cmd);
}

auto make_parser()
{
    namespace x3 = boost::spirit::x3;

    const auto noop_action = [](auto& /*ctx*/) {};
    const auto noop_parser = x3::rule<struct _noop, commands::noop>{} = x3::lit("noop")[ noop_action ];

    const auto addx_action = [](auto& ctx) { x3::_val(ctx) = commands::addx{ x3::_attr(ctx) }; };
    const auto addx_parser = x3::rule<struct _addx, commands::addx>{} = (x3::lit("addx") > x3::int_)[ addx_action ];

    const auto cmd_parser = addx_parser | noop_parser;

    return cmd_parser;
}

command parse_command(std::istream& input)
{
    std::string line;
    ensure(std::getline(input, line));

    command cmd;
    const bool parsed = aoc::x3_parse(line, make_parser(), boost::spirit::x3::space, cmd);
    if (!parsed)
        throw std::runtime_error("failed to parse a command");

    return cmd;
}

std::vector<int> calc_regx_states(std::istream& input, const size_t ticks_to_run)
{
    std::vector<int> regx_states;
    regx_states.reserve(ticks_to_run);

    int current_tick             = 1;
    size_t cmd_ticks_to_complete = 0;

    state state{};
    command cmd = commands::noop{};
    while (current_tick <= ticks_to_run)
    {
        if (cmd_ticks_to_complete == 0)
        {
            finish_execution(state, cmd);

            cmd                   = parse_command(input);
            cmd_ticks_to_complete = get_ticks_to_complete(cmd);
        }

        regx_states.push_back(state.regX);

        ++current_tick;
        --cmd_ticks_to_complete;
    }

    return regx_states;
}

}  // namespace

namespace aoc
{
namespace year_2022
{

int solve10_a(std::istream& input)
{
    constexpr size_t ticks_to_run = 220;
    const auto regx_states        = calc_regx_states(input, ticks_to_run);

    constexpr int ticks_to_sum_regx[] = { 20, 60, 100, 140, 180, 220 };

    int sum = 0;
    for (const auto tick : ticks_to_sum_regx)
    {
        sum += tick * regx_states[ tick - 1 ];
    }

    return sum;
}

std::string solve10_b(std::istream& input)
{
    constexpr size_t crt_width = 40;
    constexpr size_t crt_rows  = 6;
    constexpr size_t crt_size  = crt_width * crt_rows;

    std::string crt_pixels;
    crt_pixels.reserve(crt_size);

    const auto regx_states = calc_regx_states(input, crt_size);
    for (size_t tick = 0; tick < crt_size; ++tick)
    {
        const int crt_pos       = tick % crt_width;
        const int regx          = regx_states[ tick ];
        const bool is_pixel_set = (crt_pos == regx - 1 || crt_pos == regx || crt_pos == regx + 1);
        crt_pixels.push_back(is_pixel_set ? '#' : '.');
    }

    return crt_pixels | std::views::chunk(crt_width) | std::views::join_with('\n') | std::ranges::to<std::string>();
}

}  // namespace year_2022
}  // namespace aoc