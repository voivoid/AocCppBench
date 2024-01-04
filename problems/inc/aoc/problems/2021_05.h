#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2021
{

size_t solve05_a(std::istream& input);
size_t solve05_b(std::istream& input);

}  // namespace year_2021
}  // namespace aoc

AOC_REGISTER_PROBLEM(2021_05_a, aoc::year_2021::solve05_a, 2021, 5)
AOC_REGISTER_PROBLEM(2021_05_b, aoc::year_2021::solve05_b, 2021, 5)
