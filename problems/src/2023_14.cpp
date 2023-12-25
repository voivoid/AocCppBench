#include "aoc/problems/2023_14.h"

#include "coords.h"
#include "grid.h"

#include <cassert>
#include <istream>
#include <ranges>
#include <string>
#include <unordered_set>

namespace
{
using platform = aoc::grid;

platform tilt_platform_impl(const platform& platform, const auto indices)
{
    const auto [ horizontal_tilt, reverse ] = [ &indices ]()
    {
        const auto [ x0, y0 ]      = indices[ 0 ][ 0 ];
        const auto [ x1, y1 ]      = indices[ 0 ][ 1 ];
        const bool horizontal_tilt = x0 != x1;
        const bool reverse         = horizontal_tilt ? x1 < x0 : y1 < y0;
        return std::make_pair(horizontal_tilt, reverse);
    }();

    const size_t width  = platform.get_width();
    const size_t height = platform.get_height();

    ::platform next_state(std::string(width * height, '.'), width, height);

    for (const auto colrows : indices)
    {
        const auto [ x0, y0 ] = colrows[ 0 ];
        size_t to             = horizontal_tilt ? x0 : y0;
        for (const auto [ x, y ] : colrows)
        {
            const char tile = platform(x, y);

            if (tile == '#')
            {
                next_state(x, y) = '#';

                to = (horizontal_tilt ? x : y) + (reverse ? -1 : 1);
            }
            else if (tile == 'O')
            {
                if (horizontal_tilt) next_state(to, y) = 'O';
                else
                    next_state(x, to) = 'O';

                reverse ? --to : ++to;
            }
        }
    }

    return next_state;
}

size_t calc_north_load(const platform& platform)
{
    const size_t width  = platform.get_width();
    const size_t height = platform.get_height();

    size_t total_load = 0;

    for (size_t x = 0; x < width; ++x)
    {
        for (size_t y = 0; y < height; ++y)
        {
            if (platform(x, y) == 'O') { total_load += height - y; }
        }
    }
    return total_load;
}

platform tilt_platform(const platform& platform, const aoc::direction dir)
{
    using namespace std::ranges;

    const auto xs = views::iota(size_t(0), platform.get_width());
    const auto ys = views::iota(size_t(0), platform.get_height());

    switch (dir)
    {
        case aoc::direction::north:
            return tilt_platform_impl(
                platform, xs | views::transform([ ys ](const auto x) { return views::zip(views::repeat(x), ys); }));

        case aoc::direction::west:
            return tilt_platform_impl(
                platform, ys | views::transform([ xs ](const auto y) { return views::zip(xs, views::repeat(y)); }));

        case aoc::direction::south:
            return tilt_platform_impl(
                platform,
                xs | views::transform([ ys ](const auto x)
                                      { return views::zip(views::repeat(x), ys | views::reverse); }));

        case aoc::direction::east:
        {
            return tilt_platform_impl(
                platform,
                ys | views::transform([ xs ](const auto y)
                                      { return views::zip(xs | views::reverse, views::repeat(y)); }));
        }
    }

    std::unreachable();
}

platform run_tilt_cycles(platform platform, long long n = 1)
{
    const aoc::direction cycle_dirs[] = {
        aoc::direction::north, aoc::direction::west, aoc::direction::south, aoc::direction::east
    };

    std::unordered_set<std::string> visited;

    while (n-- > 0)
    {
        visited.insert(platform.str());
        platform = std::ranges::fold_left(cycle_dirs, platform, tilt_platform);
        if (visited.contains(platform.str())) break;
    }

    const std::string cycle_start_platform = platform.str();

    size_t cycle_len = 1;
    while (n-- > 0)
    {
        platform = std::ranges::fold_left(cycle_dirs, platform, tilt_platform);
        if (platform.str() == cycle_start_platform) break;
        ++cycle_len;
    }

    n %= cycle_len;
    while (n-- > 0) { platform = std::ranges::fold_left(cycle_dirs, platform, tilt_platform); }

    return platform;
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve14_a(std::istream& input)
{
    const auto initial_platform = aoc::grid::read_whole_input(input);
    return calc_north_load(tilt_platform(initial_platform, aoc::direction::north));
}

size_t solve14_b(std::istream& input)
{
    const auto initial_platform = aoc::grid::read_whole_input(input);
    return calc_north_load(run_tilt_cycles(initial_platform, 1000000000));
}

}  // namespace year_2023
}  // namespace aoc
