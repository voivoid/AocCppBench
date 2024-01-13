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
size_t solve07_a(std::istream& input, const char* wire);
}

size_t solve07_a(std::istream& input);
size_t solve07_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_07_a, aoc::year_2015::solve07_a, 2015, 7)
AOC_REGISTER_PROBLEM(2015_07_b, aoc::year_2015::solve07_b, 2015, 7)
