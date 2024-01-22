#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{

size_t solve15_a(std::istream& input);
size_t solve15_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_15_a, aoc::year_2015::solve15_a, 2015, 15)
AOC_REGISTER_PROBLEM(2015_15_b, aoc::year_2015::solve15_b, 2015, 15)
