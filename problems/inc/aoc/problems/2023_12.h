#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve12_a(std::istream& input);
size_t solve12_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_12_a, aoc::year_2023::solve12_a, 2023, 12)
AOC_REGISTER_PROBLEM(2023_12_b, aoc::year_2023::solve12_b, 2023, 12)
