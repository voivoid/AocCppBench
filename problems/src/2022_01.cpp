#include "aoc/problems/2022_01.h"

#include <iostream>
#include <limits>
#include <numeric>

namespace
{

size_t& min(size_t& a, size_t& b)
{
    return a <= b ? a : b;
}

size_t get_elf_calories(std::istream& input)
{
    char linebuff[ std::numeric_limits<size_t>::digits10 + 1 ]{};
    size_t calories_total = 0;
    while (input.getline(linebuff, std::size(linebuff)) && linebuff[ 0 ])
    {
        calories_total += std::atoi(linebuff);
    }

    return calories_total;
}

}  // namespace

namespace aoc
{
namespace year_2022
{

size_t solve01_a(std::istream& input)
{
    size_t max = 0;
    while (input)
    {
        max = std::max(max, get_elf_calories(input));
    }

    return max;
}

size_t solve01_b(std::istream& input)
{
    size_t top[ 3 ]{};

    while (input)
    {
        auto& min_top = min(min(top[ 0 ], top[ 1 ]), top[ 2 ]);

        const auto calories = get_elf_calories(input);

        if (calories > min_top)
            min_top = calories;
    }

    return std::accumulate(std::cbegin(top), std::cend(top), size_t(0));
}

}  // namespace year_2022
}  // namespace aoc