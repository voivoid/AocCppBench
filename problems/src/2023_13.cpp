#include "aoc/problems/2023_13.h"

#include "grid.h"

#include <algorithm>
#include <bitset>
#include <cassert>
#include <istream>
#include <numeric>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

namespace
{
using pattern = aoc::grid;

std::vector<size_t> convert_pattern_to_numbers(const pattern& pattern, auto indices)
{
    constexpr size_t max_width = sizeof(size_t) * 8;

    std::vector<size_t> numbers;
    for (const auto colrows : indices)
    {
        std::bitset<max_width> bs;

        size_t current_bit = 0;
        for (const auto [ x, y ] : colrows)
        {
            const auto c        = pattern(x, y);
            bs[ current_bit++ ] = c == '#' ? 1 : 0;
        }

        numbers.push_back(bs.to_ullong());
        bs.reset();
    }

    return numbers;
}

std::pair<std::vector<size_t>, std::vector<size_t>> convert_pattern_to_numbers(const pattern& p)
{
    using namespace std::ranges;

    const auto xs = views::iota(size_t(0), p.get_width());
    const auto ys = views::iota(size_t(0), p.get_height());

    auto rows_indices = ys | views::transform([ xs ](const auto y) { return views::zip(xs, views::repeat(y)); });
    auto cols_indices = xs | views::transform([ ys ](const auto x) { return views::zip(views::repeat(x), ys); });

    return { convert_pattern_to_numbers(p, rows_indices), convert_pattern_to_numbers(p, cols_indices) };
}

bool can_be_fixed(const size_t a, const size_t b)
{
    assert(a != b);
#ifdef _MSC_VER
    return __popcnt64(a ^ b) == 1;
#else
    return std::bitset<sizeof(a) * 8>(a ^ b).count() == 1;
#endif
}

template <bool fix_smudge>
std::optional<size_t> find_reflection(const std::vector<size_t>& numbers)
{
    for (size_t mid = 1; mid < numbers.size(); ++mid)
    {
        bool is_smudge_fixed = !fix_smudge;
        for (size_t left = mid - 1, right = mid;; --left, ++right)
        {
            if (numbers[ left ] != numbers[ right ])
            {
                if (!is_smudge_fixed && can_be_fixed(numbers[ left ], numbers[ right ])) is_smudge_fixed = true;
                else
                    break;
            }

            if ((left == 0 || right == numbers.size() - 1))
            {
                if (is_smudge_fixed) return mid;
                else
                    break;
            }
        }
    }

    return {};
}

template <bool fix_smudge>
size_t calc_score(const pattern& p)
{
    const auto [ hor_numbers, ver_numbers ] = convert_pattern_to_numbers(p);

    if (auto cols_to_the_left = find_reflection<fix_smudge>(ver_numbers); cols_to_the_left) return *cols_to_the_left;
    if (auto rows_above = find_reflection<fix_smudge>(hor_numbers); rows_above) return *rows_above * 100;

    assert(false);

    return 0;
}

template <bool fix_smudge>
size_t solve(std::istream& input)
{
    auto notes = std::ranges::views::istream<pattern>(input) | std::ranges::views::transform(calc_score<fix_smudge>);
    const auto scores = std::ranges::fold_left(notes, size_t(0), std::plus{});

    return scores;
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve13_a(std::istream& input)
{
    return solve<false>(input);
}

size_t solve13_b(std::istream& input)
{
    return solve<true>(input);
}

}  // namespace year_2023
}  // namespace aoc
