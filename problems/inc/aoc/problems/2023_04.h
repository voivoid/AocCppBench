#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve04_a(std::istream& input);
size_t solve04_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_04_a, aoc::year_2023::solve04_a, 2023, 4)
AOC_REGISTER_PROBLEM(2023_04_b, aoc::year_2023::solve04_b, 2023, 4)
