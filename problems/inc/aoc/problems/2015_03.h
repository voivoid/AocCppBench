#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{

size_t solve03_a(std::istream& input);
size_t solve03_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_03_a, aoc::year_2015::solve03_a, 2015, 3)
AOC_REGISTER_PROBLEM(2015_03_b, aoc::year_2015::solve03_b, 2015, 3)
