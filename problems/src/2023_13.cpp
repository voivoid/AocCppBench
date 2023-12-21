#include "aoc/problems/2023_13.h"

#include <algorithm>
#include <bitset>
#include <cassert>
#include <istream>
#include <mdspan>
#include <numeric>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

namespace
{
struct pattern
{
    std::string str;
    size_t width;
    size_t height;
};

std::istream& operator>>(std::istream& is, pattern& p)
{
    p.str.clear();
    p.height = 0;
    p.width  = 0;

    std::string line;
    while (std::getline(is, line) && !line.empty())
    {
        p.str.append(line);

        p.width = line.size();
        ++p.height;
    }

    if (!p.str.empty()) { is.clear(); }

    return is;
}

std::vector<size_t> convert_pattern_to_numbers(const std::mdspan<const char, std::dextents<size_t, 2>> pattern,
                                               auto indices)
{
    constexpr size_t max_width = sizeof(size_t) * 8;

    std::vector<size_t> numbers;
    for (const auto colrows : indices)
    {
        std::bitset<max_width> bs;

        size_t current_bit = 0;
        for (const auto [ y, x ] : colrows)
        {
            const auto c        = pattern[ std::array{ y, x } ];
            bs[ current_bit++ ] = c == '#' ? 1 : 0;
        }

        numbers.push_back(bs.to_ullong());
        bs.reset();
    }

    return numbers;
}

std::pair<std::vector<size_t>, std::vector<size_t>> convert_pattern_to_numbers(const pattern& p)
{
    std::mdspan data(p.str.data(), p.height, p.width);

    const auto xs = std::ranges::views::iota(size_t(0), p.width);
    const auto ys = std::ranges::views::iota(size_t(0), p.height);

    auto rows_indices =
        ys | std::ranges::views::transform(
                 [ xs ](const auto y)
                 { return xs | std::ranges::views::transform([ y ](const auto x) { return std::make_pair(y, x); }); });

    auto cols_indices =
        xs | std::ranges::views::transform(
                 [ ys ](const auto x)
                 { return ys | std::ranges::views::transform([ x ](const auto y) { return std::make_pair(y, x); }); });


    return { convert_pattern_to_numbers(data, rows_indices), convert_pattern_to_numbers(data, cols_indices) };
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
