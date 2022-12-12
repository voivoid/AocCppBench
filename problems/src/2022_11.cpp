#include "aoc/problems/2022_11.h"

#include "parse.h"

#include <functional>
#include <iostream>
#include <ranges>

#include "boost/fusion/adapted/struct.hpp"

namespace
{

using item      = size_t;
using operation = std::function<size_t(size_t)>;
struct monkey
{
    size_t id;
    std::vector<item> items;
    operation calc_worry_level;
    size_t divisible_test_value;
    size_t throw_to_if_true;
    size_t throw_to_if_false;

    size_t items_inspected = 0;
};

using monkeys = std::vector<monkey>;

}  // namespace

BOOST_FUSION_ADAPT_STRUCT(monkey, id, items, calc_worry_level, divisible_test_value, throw_to_if_true, throw_to_if_false)

namespace
{

operation make_operation(const char op, const boost::optional<size_t>& arg)
{
    std::function<size_t(size_t, size_t)> f;
    switch (op)
    {
        case '+': f = std::plus<size_t>{}; break;
        case '-': f = std::minus<size_t>{}; break;
        case '*': f = std::multiplies<size_t>{}; break;
        case '/': f = std::divides<size_t>{}; break;
        default: throw std::runtime_error("unknown op");
    }

    if (arg)
    {
        return std::bind(f, std::placeholders::_1, *arg);
    }
    else
    {
        return std::bind(f, std::placeholders::_1, std::placeholders::_1);
    }
}

}  // namespace

namespace
{

monkeys parse_monkeys(std::istream& input)
{
    namespace x3 = boost::spirit::x3;

    const auto id_parser    = aoc::x3_size_t_;
    const auto items_parser = aoc::x3_size_t_ % ',';

    const auto op_action = [](const auto& ctx)
    {
        const boost::fusion::vector<char, boost::optional<size_t>> tuple = x3::_attr(ctx);

        const auto op  = boost::fusion::at_c<0>(tuple);
        const auto arg = boost::fusion::at_c<1>(tuple);
        x3::_val(ctx)  = make_operation(op, arg);
    };
    const auto op_parser = x3::rule<struct _op, operation>{} =
        (x3::lit('=') > x3::char_("*/-+") >> x3::skip(x3::blank)[ -aoc::x3_size_t_ ])[ op_action ];
    const auto div_parser   = aoc::x3_size_t_;
    const auto throw_parser = aoc::x3_size_t_;

    const auto monkey_parser = x3::rule<struct _monkey, monkey>{} =
        id_parser > items_parser > op_parser > div_parser > throw_parser > throw_parser;
    const auto monkeys_parser = +monkey_parser;

    const auto skipper = x3::space | x3::alpha | x3::char_(':');

    monkeys monkeys;
    const bool parsed = aoc::x3_parse(input, monkeys_parser, skipper, monkeys);
    if (!parsed)
    {
        throw std::runtime_error("failed to parse monkeys");
    }

    return monkeys;
}

void run_turn(monkey& monkey, monkeys& monkeys)
{
    for (const auto& item : monkey.items)
    {
        const size_t new_worry_level = monkey.calc_worry_level(item);
        const size_t throw_item_to =
            new_worry_level % monkey.divisible_test_value == 0 ? monkey.throw_to_if_true : monkey.throw_to_if_false;
        assert(throw_item_to != monkey.id);
        monkeys[ throw_item_to ].items.push_back(new_worry_level);

        ++monkey.items_inspected;
    }
    monkey.items.clear();
}

size_t solve(std::istream& input, const bool should_div_worry_level, size_t rounds_to_run)
{
    auto monkeys = parse_monkeys(input);
    if (should_div_worry_level)
    {
        for (auto& monkey : monkeys)
        {
            monkey.calc_worry_level = [ calc = std::move(monkey.calc_worry_level) ](const size_t item) { return calc(item) / 3; };
        }
    }

    while (rounds_to_run)
    {
        for (auto& monkey : monkeys)
        {
            run_turn(monkey, monkeys);
        }
        --rounds_to_run;
    }

    std::ranges::sort(monkeys, std::greater{}, &monkey::items_inspected);

    return monkeys[ 0 ].items_inspected * monkeys[ 1 ].items_inspected;
}

}  // namespace

namespace aoc
{
namespace year_2022
{

size_t solve11_a(std::istream& input)
{
    return solve(input, true, 20);
}

size_t solve11_b(std::istream& /*input*/)
{
    return 0;
    // return solve(input, false, 10000);
}

}  // namespace year_2022
}  // namespace aoc