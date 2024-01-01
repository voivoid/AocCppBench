#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve20_a(std::istream& input);
size_t solve20_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_20_a, aoc::year_2023::solve20_a, 2023, 20)
AOC_REGISTER_PROBLEM(2023_20_b, aoc::year_2023::solve20_b, 2023, 20)
