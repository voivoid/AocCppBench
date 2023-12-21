#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve14_a(std::istream& input);
size_t solve14_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_14_a, aoc::year_2023::solve14_a, 2023, 14)
AOC_REGISTER_PROBLEM(2023_14_b, aoc::year_2023::solve14_b, 2023, 14)
