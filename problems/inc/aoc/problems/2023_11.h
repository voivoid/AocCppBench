#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve11_a(std::istream& input);
size_t solve11_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_11_a, aoc::year_2023::solve11_a, 2023, 11)
AOC_REGISTER_PROBLEM(2023_11_b, aoc::year_2023::solve11_b, 2023, 11)
