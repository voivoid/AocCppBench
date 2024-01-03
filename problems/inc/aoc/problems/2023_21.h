#pragma once

#include "aoc/problems/register.h"

#include <cstddef>
#include <iosfwd>

namespace aoc
{
namespace year_2023
{

namespace details
{
size_t solve21_a_impl(std::istream& input, size_t steps);
size_t solve21_b_impl(std::istream& input, size_t steps);
}  // namespace details

size_t solve21_a(std::istream& input);
size_t solve21_b(std::istream& input);

}  // namespace year_2023
}  // namespace aoc

AOC_REGISTER_PROBLEM(2023_21_a, aoc::year_2023::solve21_a, 2023, 21)
AOC_REGISTER_PROBLEM(2023_21_b, aoc::year_2023::solve21_b, 2023, 21)
