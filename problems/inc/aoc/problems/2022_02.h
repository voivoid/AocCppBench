#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2022
{

size_t solve02_a(std::istream& input);
size_t solve02_b(std::istream& input);

}  // namespace year_2022
}  // namespace aoc

AOC_REGISTER_PROBLEM(2022_02_a, aoc::year_2022::solve02_a, 2022, 2)
AOC_REGISTER_PROBLEM(2022_02_b, aoc::year_2022::solve02_b, 2022, 2)