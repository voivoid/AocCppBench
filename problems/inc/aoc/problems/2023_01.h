#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve01_a(std::istream& input);
size_t solve01_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_01_a, aoc::year_2023::solve01_a, 2023, 1)
AOC_REGISTER_PROBLEM(2023_01_b, aoc::year_2023::solve01_b, 2023, 1)
