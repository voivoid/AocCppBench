#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{

size_t solve20_a(std::istream& input);
size_t solve20_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_20_a, aoc::year_2015::solve20_a, 2015, 20)
AOC_REGISTER_PROBLEM(2015_20_b, aoc::year_2015::solve20_b, 2015, 20)
