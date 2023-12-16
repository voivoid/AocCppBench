#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve10_a(std::istream& input);
size_t solve10_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_10_a, aoc::year_2023::solve10_a, 2023, 10)
AOC_REGISTER_PROBLEM(2023_10_b, aoc::year_2023::solve10_b, 2023, 10)
