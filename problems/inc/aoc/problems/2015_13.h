#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{

size_t solve13_a(std::istream& input);
size_t solve13_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_13_a, aoc::year_2015::solve13_a, 2015, 13)
AOC_REGISTER_PROBLEM(2015_13_b, aoc::year_2015::solve13_b, 2015, 13)
