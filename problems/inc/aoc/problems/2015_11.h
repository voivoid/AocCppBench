#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>
#include <string>

namespace aoc
{
namespace year_2015
{

std::string solve11_a(std::istream& input);
std::string solve11_b(std::istream& input);

}  // namespace year_2015
}  // namespace aoc

AOC_REGISTER_PROBLEM(2015_11_a, aoc::year_2015::solve11_a, 2015, 11)
AOC_REGISTER_PROBLEM(2015_11_b, aoc::year_2015::solve11_b, 2015, 11)
