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
size_t solve18_a(std::istream& input, size_t steps);
size_t solve18_b(std::istream& input, size_t steps);
}  // namespace details

size_t solve18_a(std::istream& input);
size_t solve18_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_18_a, aoc::year_2015::solve18_a, 2015, 18)
AOC_REGISTER_PROBLEM(2015_18_b, aoc::year_2015::solve18_b, 2015, 18)
