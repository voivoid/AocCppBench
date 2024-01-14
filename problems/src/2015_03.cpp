#include "aoc/problems/2015_03.h"

#include "coords.h"
#include "views.h"

#include <istream>
#include <ranges>
#include <unordered_set>

namespace
{
using location = aoc::point;

constexpr location initial_location{ 0, 0 };

location::offset get_move_offset(const char c)
{
    switch (c)
    {
        case '>': return { 1, 0 };
        case '<': return { -1, 0 };
        case '^': return { 0, -1 };
        case 'v': return { 0, 1 };
    }

    std::unreachable();
}

template <size_t SantasNum>
size_t solve(std::istream& input)
{
    namespace views = std::ranges::views;

    auto move_offsets = views::istream<char>(input) | views::transform(get_move_offset);

    std::unordered_set<location> visited;
    visited.insert(initial_location);

    std::array<location, SantasNum> santas_locations;
    std::ranges::fill(santas_locations, initial_location);

    size_t current_santa = 0;
    for (const auto& offset : move_offsets)
    {
        auto& location = santas_locations[ current_santa ];
        location += offset;
        visited.insert(location);

        current_santa = (current_santa + 1) % SantasNum;
    }

    return visited.size();
}
}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve03_a(std::istream& input)
{
    return solve<1>(input);
}

size_t solve03_b(std::istream& input)
{
    return solve<2>(input);
}

}  // namespace year_2015
}  // namespace aoc
