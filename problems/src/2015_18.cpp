#include "aoc/problems/2015_18.h"

#include "grid.h"

#include <algorithm>
#include <cassert>
#include <ranges>

namespace
{
constexpr char on_tile  = '#';
constexpr char off_tile = '.';

char calc_next_state(const char current_state, const size_t neighbours_on)
{
    if (current_state == on_tile) { return (neighbours_on == 2 || neighbours_on == 3) ? on_tile : off_tile; }
    else
    {
        assert(current_state == off_tile);
        return neighbours_on == 3 ? on_tile : off_tile;
    }
}

void next_generation(aoc::grid& grid, aoc::grid& tmp_lines)
{
    const auto width    = grid.get_width();
    const auto height   = grid.get_height();
    const auto t_height = tmp_lines.get_height();

    const auto bounding_rect = grid.get_bounding_rect();

    for (size_t y = 0; y < height + 3; ++y)
    {
        const size_t t_y = y % t_height;
        if (y >= 3)
        {
            for (size_t x = 0; x < width; ++x) { grid(x, y - 3) = tmp_lines(x, t_y); }
        }

        if (y < height)
        {
            for (size_t x = 0; x < width; ++x)
            {
                const auto neighbours = aoc::get_neighbours({ x, y }, bounding_rect);

                size_t neighbours_on = 0;
                for (const auto neighbour : neighbours)
                {
                    if (grid(neighbour.x, neighbour.y) == on_tile) ++neighbours_on;
                };

                tmp_lines(x, t_y) = calc_next_state(grid(x, y), neighbours_on);
            }
        }
    }
}

void switch_corners_on(aoc::grid& grid)
{
    const auto right  = grid.get_width() - 1;
    const auto bottom = grid.get_height() - 1;

    grid(0, 0)          = on_tile;
    grid(0, bottom)     = on_tile;
    grid(right, 0)      = on_tile;
    grid(right, bottom) = on_tile;
}

template <bool corners_are_always_on>
size_t solve(std::istream& input, size_t steps)
{
    auto grid = aoc::grid::read_whole_input(input, 100);

    aoc::grid tmp_lines(grid.get_width(), 3);

    while (steps--)
    {
        if constexpr (corners_are_always_on) switch_corners_on(grid);
        next_generation(grid, tmp_lines);
    }

    if constexpr (corners_are_always_on) switch_corners_on(grid);
    return std::ranges::count(grid.get_range(), on_tile);
}
}  // namespace

namespace aoc
{
namespace year_2015
{

namespace details
{
size_t solve18_a(std::istream& input, const size_t steps)
{
    return solve<false>(input, steps);
}

size_t solve18_b(std::istream& input, const size_t steps)
{
    return solve<true>(input, steps);
}
}  // namespace details

size_t solve18_a(std::istream& input)
{
    return details::solve18_a(input, 100);
}

size_t solve18_b(std::istream& input)
{
    return details::solve18_b(input, 100);
}

}  // namespace year_2015
}  // namespace aoc
