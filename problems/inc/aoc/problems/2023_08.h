#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

size_t solve08_a(std::istream& input);
size_t solve08_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_08_a, aoc::year_2023::solve08_a, 2023, 8)
AOC_REGISTER_PROBLEM(2023_08_b, aoc::year_2023::solve08_b, 2023, 8)
