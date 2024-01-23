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
size_t solve17_a(std::istream& input, size_t vol);
size_t solve17_b(std::istream& input, size_t vol);
}  // namespace details

size_t solve17_a(std::istream& input);
size_t solve17_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_17_a, aoc::year_2015::solve17_a, 2015, 17)
AOC_REGISTER_PROBLEM(2015_17_b, aoc::year_2015::solve17_b, 2015, 17)
