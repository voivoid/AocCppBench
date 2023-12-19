#include "aoc/problems/2023_11.h"

#include "coords.h"

#include <algorithm>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>

namespace
{
using coord = size_t;
std::vector<aoc::upoint> parse_galaxies(std::istream& input)
{
    std::vector<aoc::upoint> galaxies;

    coord x = 0;
    coord y = 0;
    while (input)
    {
        char c;
        input.get(c);

        if (c != '\n')
        {
            if (c == '#')
            {
                galaxies.emplace_back(x, y);
            }

            ++x;
        }
        else
        {
            x = 0;
            ++y;
        }
    }

    return galaxies;
}

size_t calc_min_distance(const aoc::upoint p1,
                         const aoc::upoint p2,
                         const std::vector<size_t>& empty_cols_scan,
                         const std::vector<size_t>& empty_rows_scan,
                         const size_t empty_row_mul)
{
    const auto [ left, right ] = std::minmax(p1.x, p2.x);
    const auto [ top, bottom ] = std::minmax(p1.y, p2.y);

    const auto empty_cols = empty_cols_scan[ right ] - empty_cols_scan[ left ];
    const auto empty_rows = empty_rows_scan[ bottom ] - empty_rows_scan[ top ];

    const auto expansion_distance = empty_row_mul * (empty_cols + empty_rows);
    const auto basic_distance     = aoc::manhattan_distance(p1, p2);

    return basic_distance + expansion_distance;
}

std::vector<size_t> scan_no_galaxy_intervals(const std::set<coord>& galaxy_coords)
{
    const coord max_coord = *(std::prev(galaxy_coords.cend()));
    std::vector<size_t> scan(max_coord + 1);

    auto coords = std::ranges::views::iota(size_t(0), scan.size());
    std::transform_inclusive_scan(coords.cbegin(),
                                  coords.cend(),
                                  scan.begin(),
                                  std::plus<size_t>{},
                                  [ & ](const coord x) { return !galaxy_coords.contains(x) ? size_t(1) : size_t(0); });

    return scan;
}

size_t solve(std::istream& input, const size_t empty_row_mul)
{
    const auto galaxies = parse_galaxies(input);

    std::set<coord> galaxy_xs, galaxy_ys;
    for (const auto galaxy : galaxies)
    {
        galaxy_xs.insert(galaxy.x);
        galaxy_ys.insert(galaxy.y);
    }

    const std::vector<size_t> empty_cols_scan = scan_no_galaxy_intervals(galaxy_xs);
    const std::vector<size_t> empty_rows_scan = scan_no_galaxy_intervals(galaxy_ys);

    size_t distance_sum = 0;
    for (auto permut_iter1 = galaxies.cbegin(); permut_iter1 != galaxies.cend(); ++permut_iter1)
    {
        for (auto permut_iter2 = std::next(permut_iter1); permut_iter2 != galaxies.cend(); ++permut_iter2)
        {
            distance_sum += calc_min_distance(*permut_iter1, *permut_iter2, empty_cols_scan, empty_rows_scan, empty_row_mul);
        }
    }

    return distance_sum;
}
}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve11_a(std::istream& input)
{
    return solve(input, 1);
}

size_t solve11_b(std::istream& input)
{
    return solve(input, 999999);
}

}  // namespace year_2023
}  // namespace aoc
