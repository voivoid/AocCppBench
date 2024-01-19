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
size_t solve14_a(std::istream& input, size_t seconds);
size_t solve14_b(std::istream& input, size_t seconds);
}  // namespace details

size_t solve14_a(std::istream& input);
size_t solve14_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_14_a, aoc::year_2015::solve14_a, 2015, 14)
AOC_REGISTER_PROBLEM(2015_14_b, aoc::year_2015::solve14_b, 2015, 14)
