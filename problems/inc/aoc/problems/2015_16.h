#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{

size_t solve16_a(std::istream& input);
size_t solve16_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_16_a, aoc::year_2015::solve16_a, 2015, 16)
AOC_REGISTER_PROBLEM(2015_16_b, aoc::year_2015::solve16_b, 2015, 16)
