#include "aoc/problems/2023_03.h"

#include "coords.h"
#include "ensure.h"

#include <algorithm>
#include <istream>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

#include <boost/container/static_vector.hpp>

namespace
{
using schematic = std::vector<std::string>;

schematic read_schematic(std::istream& input)
{
    schematic schematic;

    std::string line;
    while (std::getline(input, line)) { schematic.push_back(std::move(line)); }

    return schematic;
}

size_t find_numbers_first_digit_x_coord(const schematic& schematic, size_t x, const size_t y)
{
    assert(std::isdigit(schematic[ y ][ x ]));

    const auto width = schematic[ 0 ].size();
    while (x < width && std::isdigit(schematic[ y ][ x + 1 ])) { ++x; }

    return x;
}

size_t read_number(const schematic& schematic, const size_t number_rightmost_x, const size_t y)
{
    assert(std::isdigit(schematic[ y ][ number_rightmost_x ]));

    auto x        = number_rightmost_x;
    size_t number = 0;
    size_t mul    = 1;

    while (true)
    {
        auto digit = schematic[ y ][ x ];
        if (!std::isdigit(digit)) break;

        number += mul * (digit - '0');
        mul *= 10;

        if (x-- == 0) break;
    };

    return number;
}

boost::container::static_vector<size_t, 9>
    get_adjacent_numbers(const schematic& schematic, const size_t x, const size_t y)
{
    const auto width  = schematic[ 0 ].size();
    const auto height = schematic.size();

    std::unordered_set<aoc::upoint> numbers_rightmost_coords;  // use unordered_set to get rid of possible rightmost
                                                               // coord duplicates
    for (const auto neighbour_coord : aoc::get_neighbours(x, y, { { 0, 0 }, { width, height } }))
    {
        const auto neighbour_char = schematic[ neighbour_coord.y ][ neighbour_coord.x ];
        if (std::isdigit(neighbour_char))
        {
            const auto number_x = find_numbers_first_digit_x_coord(schematic, neighbour_coord.x, neighbour_coord.y);
            numbers_rightmost_coords.emplace(number_x, neighbour_coord.y);
        }
    }

    boost::container::static_vector<size_t, 9> numbers;

    for (const auto& number_start_coord : numbers_rightmost_coords)
    {
        numbers.push_back(read_number(schematic, number_start_coord.x, number_start_coord.y));
    }

    return numbers;
}

template <auto calc_adjacent_numbers>
size_t solve(std::istream& input)
{
    const auto schematic = read_schematic(input);

    size_t sum = 0;
    for (const auto& [ y, row ] : schematic | std::ranges::views::enumerate)
    {
        for (const auto& [ x, symbol ] : row | std::ranges::views::enumerate)
        {
            sum += calc_adjacent_numbers(schematic, symbol, x, y);
        }
    }

    return sum;
}

size_t calc_adjacent_numbers_a(const schematic& schematic, const char symbol, const size_t x, const size_t y)
{
    if (symbol != '.' && !std::isdigit(symbol))
    {
        const auto adjacent_numbers = get_adjacent_numbers(schematic, x, y);
        return std::ranges::fold_left(adjacent_numbers, 0, std::plus{});
    }

    return 0;
}

size_t calc_adjacent_numbers_b(const schematic& schematic, const char symbol, const size_t x, const size_t y)
{
    if (symbol == '*')
    {
        const auto adjacent_numbers = get_adjacent_numbers(schematic, x, y);
        if (adjacent_numbers.size() == 2) { return adjacent_numbers[ 0 ] * adjacent_numbers[ 1 ]; }
    }

    return 0;
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve03_a(std::istream& input)
{
    return solve<&calc_adjacent_numbers_a>(input);
}

size_t solve03_b(std::istream& input)
{
    return solve<&calc_adjacent_numbers_b>(input);
}

}  // namespace year_2023
}  // namespace aoc