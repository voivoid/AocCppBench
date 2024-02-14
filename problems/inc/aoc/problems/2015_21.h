#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{
size_t solve21_a(std::istream& input);
size_t solve21_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_21_a, aoc::year_2015::solve21_a, 2015, 21)
AOC_REGISTER_PROBLEM(2015_21_b, aoc::year_2015::solve21_b, 2015, 21)
