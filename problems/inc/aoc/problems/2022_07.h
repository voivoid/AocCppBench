#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2022
{

size_t solve07_a(std::istream& input);
size_t solve07_b(std::istream& input);

}  // namespace year_2022
}  // namespace aoc

AOC_REGISTER_PROBLEM(2022_07_a, aoc::year_2022::solve07_a, 2022, 7)
AOC_REGISTER_PROBLEM(2022_07_b, aoc::year_2022::solve07_b, 2022, 7)