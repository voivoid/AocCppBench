#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{

size_t solve23_a(std::istream& input);
size_t solve23_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_23_a, aoc::year_2015::solve23_a, 2015, 23)
AOC_REGISTER_PROBLEM(2015_23_b, aoc::year_2015::solve23_b, 2015, 23)
