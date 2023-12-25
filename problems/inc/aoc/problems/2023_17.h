#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve17_a(std::istream& input);
size_t solve17_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_17_a, aoc::year_2023::solve17_a, 2023, 17)
AOC_REGISTER_PROBLEM(2023_17_b, aoc::year_2023::solve17_b, 2023, 17)
