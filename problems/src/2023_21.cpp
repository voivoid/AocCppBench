#include "aoc/problems/2023_21.h"

#include "coords.h"
#include "grid.h"

#include <unordered_set>

namespace aoc
{
namespace year_2023
{
namespace details
{
size_t solve21_a_impl(std::istream& input, size_t steps)
{
    const auto grid                = aoc::grid::read_whole_input(input);
    const aoc::urect bounding_rect = grid.get_bounding_rect();

    const auto start = grid.find('S');
    assert(start);

    std::unordered_set<aoc::upoint> plots_reachable;
    plots_reachable.insert(*start);

    while (steps--)
    {
        std::unordered_set<aoc::upoint> next_step_plots;

        for (const auto pos : plots_reachable)
        {
            for (const auto neighbour : aoc::get_neighbours<false>(pos, bounding_rect))
            {
                if (grid(neighbour.x, neighbour.y) == '#') continue;

                next_step_plots.insert(neighbour);
            }
        }

        plots_reachable = std::move(next_step_plots);
    }


    return plots_reachable.size();
}

size_t solve21_b_impl(std::istream& /*input*/, size_t /*steps*/)
{
    return 0;
}

}  // namespace details

size_t solve21_a(std::istream& input)
{
    return details::solve21_a_impl(input, 64);
}

size_t solve21_b(std::istream& input)
{
    return details::solve21_b_impl(input, 26501365);
}

}  // namespace year_2023
}  // namespace aoc
