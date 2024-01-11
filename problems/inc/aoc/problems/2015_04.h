#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2015
{
size_t solve04_a(std::istream& input);
size_t solve04_b(std::istream& input);

size_t solve04_test(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_04_a, aoc::year_2015::solve04_a, 2015, 4)
AOC_REGISTER_PROBLEM(2015_04_b, aoc::year_2015::solve04_b, 2015, 4)
