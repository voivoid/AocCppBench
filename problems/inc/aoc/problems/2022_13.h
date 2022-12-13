#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>
#include <string>

namespace aoc
{
namespace year_2022
{

size_t solve13_a(std::istream& input);
size_t solve13_b(std::istream& input);

}  // namespace year_2022
}  // namespace aoc

AOC_REGISTER_PROBLEM(2022_13_a, aoc::year_2022::solve13_a, 2022, 13)
AOC_REGISTER_PROBLEM(2022_13_b, aoc::year_2022::solve13_b, 2022, 13)