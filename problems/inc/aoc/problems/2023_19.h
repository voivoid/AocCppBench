#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve19_a(std::istream& input);
size_t solve19_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_19_a, aoc::year_2023::solve19_a, 2023, 19)
AOC_REGISTER_PROBLEM(2023_19_b, aoc::year_2023::solve19_b, 2023, 19)
