#include "aoc/problems/2022_05.h"

#include "parse.h"

#include <cassert>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

#include "boost/fusion/adapted/struct.hpp"

namespace
{
using crate        = char;
using crate_stack  = std::deque<crate>;
using crate_stacks = std::vector<crate_stack>;

struct command
{
    size_t to_move;
    size_t from;
    size_t to;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT(command, to_move, from, to)

namespace
{

void parse_crates_line(const std::string& line, crate_stacks& stacks)
{
    const auto stack_columns = stacks.size();
    for (size_t i = 0; i < stack_columns; ++i)
    {
        const auto crate_id_idx = i * 4 + 1;
        assert(crate_id_idx < line.size());

        const char id = line[ crate_id_idx ];
        if (std::isalpha(id))
            stacks[ i ].push_front(id);
    }
}

crate_stacks parse_starting_crates(std::istream& input)
{
    crate_stacks stacks;

    std::string line;
    while (std::getline(input, line))
    {
        if (stacks.empty())
        {
            assert((line.size() + 1) % 4 == 0);
            const size_t columns = (line.size() + 1) / 4;
            stacks.resize(columns);
        }

        if (line.empty())
            break;

        parse_crates_line(line, stacks);
    }

    return stacks;
}

auto make_parser()
{
    namespace x3 = boost::spirit::x3;

    const auto cmd_parser = x3::rule<struct _cmd, command>{} =
        x3::lit("move") > aoc::x3_size_t_ > x3::lit("from") > aoc::x3_size_t_ > x3::lit("to") > aoc::x3_size_t_;
    const auto cmd_action = [](auto& ctx)
    {
        --x3::_attr(ctx).from;
        --x3::_attr(ctx).to;
    };

    return cmd_parser[ cmd_action ];
}

void move_1_crate(const size_t from, const size_t to, crate_stacks& stacks)
{
    const auto crate = stacks[ from ].back();
    stacks[ from ].pop_back();
    stacks[ to ].push_back(crate);
}

void exec_command_9000(const command& cmd, crate_stacks& stacks)
{
    for (size_t i = 0; i < cmd.to_move; ++i)
    {
        move_1_crate(cmd.from, cmd.to, stacks);
    }
}

void exec_command_9001(const command& cmd, crate_stacks& stacks)
{
    auto& from_stack = stacks[ cmd.from ];
    auto& to_stack   = stacks[ cmd.to ];

    const auto from_stack_begin = from_stack.begin() + (from_stack.size() - cmd.to_move);

    to_stack.insert(to_stack.end(), from_stack_begin, from_stack.end());
    from_stack.erase(from_stack_begin, from_stack.end());
}

std::string get_top_crates(const crate_stacks& stacks)
{
    std::string result;
    result.reserve(stacks.size());
    for (const auto& stack : stacks)
    {
        if (!stack.empty())
            result.push_back(stack.back());
    }

    return result;
}

template <void (*exec)(const command&, crate_stacks&)>
std::string solve(std::istream& input)
{
    crate_stacks stacks = parse_starting_crates(input);

    aoc::parse_each_line_and_exec(input, make_parser(), [ &stacks ](const command& cmd) { exec(cmd, stacks); });

    const std::string top_crates = get_top_crates(stacks);
    return top_crates;
}

}  // namespace

namespace aoc
{
namespace year_2022
{

std::string solve05_a(std::istream& input)
{
    return solve<exec_command_9000>(input);
}

std::string solve05_b(std::istream& input)
{
    return solve<exec_command_9001>(input);
}

}  // namespace year_2022
}  // namespace aoc