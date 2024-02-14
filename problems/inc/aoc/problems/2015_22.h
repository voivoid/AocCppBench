#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{

size_t solve22_a(std::istream& input);
size_t solve22_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_22_a, aoc::year_2015::solve22_a, 2015, 22)
AOC_REGISTER_PROBLEM(2015_22_b, aoc::year_2015::solve22_b, 2015, 22)
