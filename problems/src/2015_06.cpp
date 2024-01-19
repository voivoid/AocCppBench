#include "aoc/problems/2015_06.h"

#include "coords.h"
#include "coords_fusion.h"
#include "grid.h"
#include "parse.h"

#include <algorithm>
#include <istream>
#include <ranges>

namespace
{
struct instruction
{
    enum class behavior
    {
        turn_on,
        turn_off,
        toggle
    };

    behavior mode;
    aoc::urect area;
};

using lamp = unsigned short;
using grid = aoc::generic_grid<lamp>;
}  // namespace

BOOST_FUSION_ADAPT_STRUCT(instruction, mode, area)

namespace
{
auto make_parser()
{
    namespace x3 = boost::spirit::x3;

    const x3::symbols<instruction::behavior> mode_symbols{ { "turn on", instruction::behavior::turn_on },
                                                           { "turn off", instruction::behavior::turn_off },
                                                           { "toggle", instruction::behavior::toggle } };

    static constexpr auto area_action = [](const auto& ctx)
    {
        const auto [width, height] = aoc::x3_attrs_tuple(ctx);
        x3::_val(ctx)     = aoc::urect{ width, height };
    };

    const auto point_parser = x3::rule<struct _point, aoc::upoint>{} = aoc::x3_size_t_ > ',' > aoc::x3_size_t_;

    const auto area_parser = x3::rule<struct _rect, aoc::urect>{} =
        (point_parser > "through" > point_parser)[ area_action ];

    const auto instruction_parser = x3::rule<struct _instr, instruction>{} = mode_symbols > area_parser;

    return instruction_parser;
}

template <auto manipulator>
void run_instruction(grid& lamps, const aoc::urect& area)
{
    for (size_t x = area.left; x <= area.right; ++x)
    {
        for (size_t y = area.top; y <= area.bottom; ++y) { manipulator(lamps(x, y)); }
    }
}

auto make_instruction_runner1(const instruction::behavior mode)
{
    using enum instruction::behavior;
    switch (mode)
    {
        case turn_on: return run_instruction<[](lamp& l) { l = 1; }>;
        case turn_off: return run_instruction<[](lamp& l) { l = 0; }>;
        case toggle: return run_instruction<[](lamp& l) { l ^= 1; }>;
    }

    std::unreachable();
}

auto make_instruction_runner2(const instruction::behavior mode)
{
    using enum instruction::behavior;
    switch (mode)
    {
        case turn_on: return run_instruction<[](lamp& l) { ++l; }>;
        case turn_off: return run_instruction<[](lamp& l) { l -= !!l; }>;
        case toggle: return run_instruction<[](lamp& l) { l += 2; }>;
    }

    std::unreachable();
}

size_t calc_total_brightness(const grid& lamps)
{
    return std::ranges::fold_left(lamps.get_range(), size_t(0), std::plus{});
}

template <auto instruction_runner_factory>
size_t solve(std::istream& input)
{
    constexpr size_t side = 1000;
    grid lamps{ side, side };

    for (const auto& instruction : aoc::parse_lines<instruction>(input, make_parser()))
    {
        const auto instruction_runner = instruction_runner_factory(instruction.mode);
        instruction_runner(lamps, instruction.area);
    }

    return calc_total_brightness(lamps);
}
}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve06_a(std::istream& input)
{
    return solve<make_instruction_runner1>(input);
}

size_t solve06_b(std::istream& input)
{
    return solve<make_instruction_runner2>(input);
}

}  // namespace year_2015
}  // namespace aoc
