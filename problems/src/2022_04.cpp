#include "aoc/problems/2022_04.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>

namespace
{
using range = std::pair<size_t, size_t>;

auto make_parser()
{

    namespace x3 = boost::spirit::x3;

    const auto range_parser = aoc::x3_size_t_ > x3::lit('-') > aoc::x3_size_t_;
    const auto pair_parser  = range_parser > x3::lit(',') > range_parser;

    return pair_parser;
}

template <bool (*overlap_check)(const range&, const range&)>
size_t solve(std::istream& input)
{
    size_t overlaps = 0;

    aoc::parse_each_line_and_exec(input,
                                  make_parser(),
                                  [ &overlaps ](const std::tuple<size_t, size_t, size_t, size_t>& pair)
                                  {
                                      const auto [ from1, to1, from2, to2 ] = pair;
                                      if (overlap_check(std::make_pair(from1, to1), std::make_pair(from2, to2)))
                                          ++overlaps;
                                  });

    return overlaps;
}

bool contains(const range& r1, const range& r2)
{
    return r1.first <= r2.first && r1.second >= r2.second;
}


bool fully_contains(const range& r1, const range& r2)
{
    return contains(r1, r2) || contains(r2, r1);
}

bool overlaps(const range& r1, const range& r2)
{
    return (r1.second >= r2.first && r1.second <= r2.second) || (r1.first >= r2.first && r1.first <= r2.second) || contains(r1, r2);
}
}  // namespace


namespace aoc
{
namespace year_2022
{

size_t solve04_a(std::istream& input)
{
    return solve<fully_contains>(input);
}

size_t solve04_b(std::istream& input)
{
    return solve<overlaps>(input);
}

}  // namespace year_2022
}  // namespace aoc