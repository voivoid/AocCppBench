#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>
#include <string>

namespace aoc
{
namespace year_2022
{

std::string solve05_a(std::istream& input);
std::string solve05_b(std::istream& input);

}  // namespace year_2022
}  // namespace aoc

AOC_REGISTER_PROBLEM(2022_05_a, aoc::year_2022::solve05_a, 2022, 5)
AOC_REGISTER_PROBLEM(2022_05_b, aoc::year_2022::solve05_b, 2022, 5)