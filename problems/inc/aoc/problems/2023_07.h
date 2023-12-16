#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve07_a(std::istream& input);
size_t solve07_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_07_a, aoc::year_2023::solve07_a, 2023, 7)
AOC_REGISTER_PROBLEM(2023_07_b, aoc::year_2023::solve07_b, 2023, 7)
