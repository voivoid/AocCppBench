#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>
#include <string>

namespace aoc
{
namespace year_2022
{

size_t solve12_a(std::istream& input);
size_t solve12_b(std::istream& input);

}  // namespace year_2022
}  // namespace aoc

AOC_REGISTER_PROBLEM(2022_12_a, aoc::year_2022::solve12_a, 2022, 12)
AOC_REGISTER_PROBLEM(2022_12_b, aoc::year_2022::solve12_b, 2022, 12)