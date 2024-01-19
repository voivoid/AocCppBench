#include "aoc/problems/2023_02.h"

#include "ensure.h"
#include "parse.h"

#include <istream>
#include <ranges>
#include <vector>

#include <boost/fusion/adapted/struct.hpp>

namespace
{
struct set
{
    size_t red   = 0;
    size_t green = 0;
    size_t blue  = 0;
};

struct game
{
    size_t id;
    std::vector<set> sets;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT(game, id, sets)

namespace
{

auto make_game_parser()
{
    namespace x3 = boost::spirit::x3;

    const boost::spirit::x3::symbols<size_t set::*> color_symbols = { { "red", &set::red },
                                                                      { "green", &set::green },
                                                                      { "blue", &set::blue } };

    const auto set_parser_action = [](const auto& ctx)
    {
        for (const auto& attr : _attr(ctx))
        {
            const auto [id, color_mptr] = aoc::fusion_to_std_tuple(attr);
            _val(ctx).*color_mptr = id;
        }
    };

    const auto color_parser = aoc::x3_size_t_ > color_symbols;
    const auto set_parser = x3::rule<struct _set, set>{} = (color_parser % ',')[ set_parser_action ];
    const auto game_parser                               = x3::rule<struct _game, game>{} =
        x3::lit("Game") > aoc::x3_size_t_ > x3::lit(':') > (set_parser % ';');

    return game_parser;
}

bool is_valid_game(const game& game)
{
    for (const auto& set : game.sets)
    {
        if (set.red > 12 || set.green > 13 || set.blue > 14) return false;
    }

    return true;
}

size_t calc_game1(const game& game)
{
    return is_valid_game(game) ? game.id : 0;
}

size_t calc_game2(const game& game)
{
    set max_set{};
    for (const auto& set : game.sets)
    {
        max_set.blue  = std::max(max_set.blue, set.blue);
        max_set.green = std::max(max_set.green, set.green);
        max_set.red   = std::max(max_set.red, set.red);
    }

    return max_set.blue * max_set.green * max_set.red;
}

template <auto calc_game>
size_t solve(std::istream& input)
{
    auto games  = aoc::parse_lines<game>(input, make_game_parser());
    auto powers = games | std::ranges::views::transform(calc_game);
    auto sum    = std::ranges::fold_left(powers, size_t(0), std::plus{});
    return sum;
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve02_a(std::istream& input)
{
    return solve<&calc_game1>(input);
}

size_t solve02_b(std::istream& input)
{
    return solve<&calc_game2>(input);
}

}  // namespace year_2023
}  // namespace aoc