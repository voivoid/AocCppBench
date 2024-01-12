#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{

size_t solve05_a(std::istream& input);
size_t solve05_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_05_a, aoc::year_2015::solve05_a, 2015, 5)
AOC_REGISTER_PROBLEM(2015_05_b, aoc::year_2015::solve05_b, 2015, 5)
