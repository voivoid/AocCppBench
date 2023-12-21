#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve13_a(std::istream& input);
size_t solve13_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_13_a, aoc::year_2023::solve13_a, 2023, 13)
AOC_REGISTER_PROBLEM(2023_13_b, aoc::year_2023::solve13_b, 2023, 13)
