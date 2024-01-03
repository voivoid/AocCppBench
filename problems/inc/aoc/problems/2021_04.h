#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2021
{

size_t solve04_a(std::istream& input);
size_t solve04_b(std::istream& input);

}  // namespace year_2021
}  // namespace aoc

AOC_REGISTER_PROBLEM(2021_04_a, aoc::year_2021::solve04_a, 2021, 4)
AOC_REGISTER_PROBLEM(2021_04_b, aoc::year_2021::solve04_b, 2021, 4)
