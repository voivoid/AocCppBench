#include "aoc/problems/2023_18.h"

#include "coords.h"
#include "parse.h"

#include <algorithm>
#include <istream>
#include <queue>
#include <string>
#include <unordered_set>

#include <boost/fusion/adapted/struct.hpp>

namespace
{
struct instruction
{
    aoc::direction dir;
    size_t steps;
    size_t alt_steps;    
    aoc::direction alt_dir;
    
};
}  // namespace

BOOST_FUSION_ADAPT_STRUCT(instruction, dir, steps, alt_steps, alt_dir)

namespace
{

auto make_parser()
{
    namespace x3 = boost::spirit::x3;

    x3::symbols<aoc::direction> dir_parser{
        { "R", aoc::direction::right },
        { "L", aoc::direction::left },
        { "U", aoc::direction::up },
        { "D", aoc::direction::down },
    };

    x3::symbols<aoc::direction> alt_dir_parser{
        { "0", aoc::direction::right },
        { "1", aoc::direction::down },
        { "2", aoc::direction::left },
        { "3", aoc::direction::up },
    };

    const auto alt_steps_parser_action = [](const auto& ctx) {
        std::string n = x3::_attr(ctx);
        x3::_val(ctx) = std::stoull(n, nullptr, 16);
    };
    const auto alt_steps_parser = x3::rule<struct _alt_steps, size_t>{} = (x3::repeat(5)[x3::xdigit])[alt_steps_parser_action];

    const auto color_parser = x3::lit("(#") > alt_steps_parser > alt_dir_parser > x3::lit(')');
    return dir_parser > aoc::x3_size_t_ > color_parser;
}

template <bool use_alt_data>
aoc::point calc_next_point(aoc::point p1, const instruction& instr)
{
    return aoc::get_next_pos(p1, use_alt_data ? instr.alt_dir : instr.dir, static_cast<long long>(use_alt_data ? instr.alt_steps : instr.steps));
}

template <bool use_alt_data>
size_t solve(std::istream& input)
{
    auto instructions = aoc::parse_lines<instruction>(input, make_parser());

    aoc::point p1{ 0, 0 };
    
    long long total_area = 0;
    size_t border = 0;
    for (const auto& instr : instructions)
    {
        aoc::point p2 = calc_next_point< use_alt_data>(p1, instr);

        if (p1.x == p2.x)
        {
            const auto [y1, y2] = std::minmax(p1.y, p2.y);
            border += y2 - y1;
        }
        else
        {
            assert(p1.y == p2.y);
            const auto [x1, x2] = std::minmax(p1.x, p2.x);
            border += x2 - x1;
        }

        total_area += (p1.y + p2.y) * (p1.x - p2.x); // Shoelace formula
        p1 = p2;
    }

    total_area /= 2;

    assert(p1.x == 0);
    assert(p1.y == 0);

    const auto interior = total_area - (border / 2) + 1; // Pick's theorem

    return border + interior;
}


}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve18_a(std::istream& input)
{
    return solve<false>(input);
}

size_t solve18_b(std::istream& input)
{
    return solve<true>(input);
}

}  // namespace year_2023
}  // namespace aoc
