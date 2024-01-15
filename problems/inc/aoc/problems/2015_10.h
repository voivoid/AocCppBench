#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{
namespace details
{
size_t solve10(std::istream& input, size_t steps);
}

size_t solve10_a(std::istream& input);
size_t solve10_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_10_a, aoc::year_2015::solve10_a, 2015, 10)
AOC_REGISTER_PROBLEM(2015_10_b, aoc::year_2015::solve10_b, 2015, 10)
