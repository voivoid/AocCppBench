#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{

size_t solve02_a(std::istream& input);
size_t solve02_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_02_a, aoc::year_2015::solve02_a, 2015, 2)
AOC_REGISTER_PROBLEM(2015_02_b, aoc::year_2015::solve02_b, 2015, 2)
