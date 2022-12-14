#include "aoc/problems/2022_14.h"

#include "coord_fusion.h"
#include "parse.h"

#include <functional>
#include <iostream>
#include <ranges>
#include <unordered_set>

namespace
{
using line = std::vector<aoc::pos>;
using cave = std::unordered_set<aoc::pos>;
static constexpr aoc::pos start_pos{ 500, 0 };

auto make_parser()
{
    namespace x3 = boost::spirit::x3;

    const auto point_parser = x3::rule<struct _pos, aoc::pos>{} = aoc::x3_size_t_ > ',' > aoc::x3_size_t_;
    const auto points_parser                                    = point_parser % "->";

    return points_parser;
}

void make_rocks(cave& cave, const line& line)
{
    assert(line.size() >= 2);

    std::optional<aoc::pos> prev_pos;
    for (const auto& pos : line)
    {
        if (prev_pos)
        {
            const bool is_horizontal = prev_pos->x == pos.x;
            if (is_horizontal)
            {
                const auto [ from_y, to_y ] = std::minmax(prev_pos->y, pos.y);
                for (size_t y = from_y; y <= to_y; ++y)
                {
                    cave.emplace(pos.x, y);
                }
            }
            else
            {
                assert(prev_pos->y == pos.y);
                const auto [ from_x, to_x ] = std::minmax(prev_pos->x, pos.x);
                for (size_t x = from_x; x <= to_x; ++x)
                {
                    cave.emplace(x, pos.y);
                }
            }
        }

        prev_pos = pos;
    }
}

cave parse_cave(std::istream& input)
{
    cave cave;
    aoc::parse_each_line_and_exec(input, make_parser(), [ &cave ](const line& line) { make_rocks(cave, line); });

    return cave;
}

aoc::pos down(const aoc::pos& p)
{
    return { p.x, p.y + 1 };
}

aoc::pos down_left(const aoc::pos& p)
{
    return { p.x - 1, p.y + 1 };
}

aoc::pos down_right(const aoc::pos& p)
{
    return { p.x + 1, p.y + 1 };
}

template <aoc::pos (*calc_next_pos)(const aoc::pos& p)>
bool try_flow_down(aoc::pos& pos, const cave& cave)
{
    const auto next_pos = calc_next_pos(pos);
    if (!cave.contains(next_pos))
    {
        pos = next_pos;
        return true;
    }

    return false;
}

bool try_flow_down(aoc::pos& pos, const cave& cave)
{
    return try_flow_down<down>(pos, cave) || try_flow_down<down_left>(pos, cave) || try_flow_down<down_right>(pos, cave);
}

bool pour_sand_until_abyss(cave& cave, const size_t abyss)
{
    aoc::pos sand_pos = start_pos;
    while (sand_pos.y < abyss)
    {
        if (!try_flow_down(sand_pos, cave))
        {
            cave.insert(sand_pos);
            return true;
        }
    }

    return false;
}

bool pour_sand_until_no_more_space(cave& cave, const size_t floor)
{
    aoc::pos sand_pos = start_pos;
    while (true)
    {
        if (try_flow_down(sand_pos, cave) && sand_pos.y != floor)
        {
            continue;
        }
        else if (sand_pos == start_pos)
        {
            return false;
        }
        else
        {
            cave.insert(sand_pos);
            return true;
        }
    }
}

size_t calc_abyss(const cave& cave)
{
    return std::ranges::max_element(cave, {}, &aoc::pos::y)->y;
}

size_t calc_floor(const cave& cave)
{
    return calc_abyss(cave) + 1;
}

template <bool (*pour_sand)(cave&, size_t), size_t (*calc_edge)(const cave&)>
size_t solve(std::istream& input)
{
    auto cave       = parse_cave(input);
    const auto edge = calc_edge(cave);

    size_t sand_came_to_rest = 0;
    while (pour_sand(cave, edge))
    {
        ++sand_came_to_rest;
    }

    return sand_came_to_rest;
}


}  // namespace


namespace aoc
{
namespace year_2022
{

size_t solve14_a(std::istream& input)
{
    return solve<pour_sand_until_abyss, calc_abyss>(input);
}

size_t solve14_b(std::istream& input)
{
    return solve<pour_sand_until_no_more_space, calc_floor>(input) + 1;
}

}  // namespace year_2022
}  // namespace aoc
