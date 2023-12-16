#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve06_a(std::istream& input);
size_t solve06_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_06_a, aoc::year_2023::solve06_a, 2023, 6)
AOC_REGISTER_PROBLEM(2023_06_b, aoc::year_2023::solve06_b, 2023, 6)
