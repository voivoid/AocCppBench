#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve09_a(std::istream& input);
size_t solve09_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_09_a, aoc::year_2023::solve09_a, 2023, 9)
AOC_REGISTER_PROBLEM(2023_09_b, aoc::year_2023::solve09_b, 2023, 9)
