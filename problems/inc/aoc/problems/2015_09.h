#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{

size_t solve09_a(std::istream& input);
size_t solve09_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_09_a, aoc::year_2015::solve09_a, 2015, 9)
AOC_REGISTER_PROBLEM(2015_09_b, aoc::year_2015::solve09_b, 2015, 9)
