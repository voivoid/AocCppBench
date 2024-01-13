#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{

size_t solve08_a(std::istream& input);
size_t solve08_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_08_a, aoc::year_2015::solve08_a, 2015, 8)
AOC_REGISTER_PROBLEM(2015_08_b, aoc::year_2015::solve08_b, 2015, 8)
