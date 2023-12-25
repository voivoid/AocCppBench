#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve16_a(std::istream& input);
size_t solve16_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_16_a, aoc::year_2023::solve16_a, 2023, 16)
AOC_REGISTER_PROBLEM(2023_16_b, aoc::year_2023::solve16_b, 2023, 16)
