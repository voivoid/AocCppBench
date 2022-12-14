#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>
#include <string>

namespace aoc
{
namespace year_2022
{

size_t solve14_a(std::istream& input);
size_t solve14_b(std::istream& input);

}  // namespace year_2022
}  // namespace aoc

AOC_REGISTER_PROBLEM(2022_14_a, aoc::year_2022::solve14_a, 2022, 14)
AOC_REGISTER_PROBLEM(2022_14_b, aoc::year_2022::solve14_b, 2022, 14)