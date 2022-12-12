#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>
#include <string>

namespace aoc
{
namespace year_2022
{

int solve10_a(std::istream& input);
std::string solve10_b(std::istream& input);

}  // namespace year_2022
}  // namespace aoc

AOC_REGISTER_PROBLEM(2022_10_a, aoc::year_2022::solve10_a, 2022, 10)
AOC_REGISTER_PROBLEM(2022_10_b, aoc::year_2022::solve10_b, 2022, 10)