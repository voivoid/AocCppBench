#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2021
{

int solve02_a(std::istream& input);
int solve02_b(std::istream& input);

}  // namespace year_2021
}  // namespace aoc

AOC_REGISTER_PROBLEM(2021_02_a, aoc::year_2021::solve02_a, 2021, 2)
AOC_REGISTER_PROBLEM(2021_02_b, aoc::year_2021::solve02_b, 2021, 2)
