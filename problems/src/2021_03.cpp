#include "aoc/problems/2021_03.h"

#include "ensure.h"
#include "parse.h"
#include "stream.h"

#include <algorithm>
#include <istream>
#include <ranges>
#include <string>
#include <vector>

#include <boost/container/static_vector.hpp>

namespace
{

using number              = unsigned;
constexpr size_t bits_num = sizeof(number) * 8;

using bits_array = boost::container::static_vector<size_t, bits_num>;


size_t bits_array_to_number(const bits_array& bits_arr)
{
    size_t n = 0;
    for (const auto bit : bits_arr | std::ranges::views::reverse)
    {
        n <<= 1;
        n |= bit;
    }

    return n;
}

size_t filter_until_one(std::vector<std::string>& nums, const char bit_criteria)
{
    assert(bit_criteria == '1' || bit_criteria == '0');

    size_t bit_idx = 0;

    auto from = nums.begin();
    auto to   = nums.end();
    for (;;)
    {
        auto bit_criteria_part_end = std::partition(
            from, to, [ bit_idx, bit_criteria ](const auto& bits_str) { return bits_str[ bit_idx ] == bit_criteria; });

        const auto bit_criteria_part = std::distance(from, bit_criteria_part_end);
        const auto other_part        = std::distance(bit_criteria_part_end, to);

        if (bit_criteria_part + other_part == 1) { return std::stoull(*from, nullptr, 2); }
        else if (bit_criteria == '1' ? (bit_criteria_part >= other_part) : (bit_criteria_part <= other_part))
        {
            to = bit_criteria_part_end;
        }
        else { from = bit_criteria_part_end; }

        ++bit_idx;
    }
}

}  // namespace

namespace aoc
{
namespace year_2021
{

size_t solve03_a(std::istream& input)
{
    const auto bits_width = aoc::peek_line_width(input);
    ensure(bits_width);

    bits_array bit1_occurences(*bits_width);

    size_t lines = 0;
    for (auto n : aoc::parse_lines<number>(input, boost::spirit::x3::bin))
    {
        for (size_t bit = 0; n; ++bit, n /= 2)
        {
            if (n % 2) ++bit1_occurences[ bit ];
        }

        ++lines;
    }

    const auto half_of_lines = lines / 2 + (lines % 2);

    for (auto& bit1_occurrence : bit1_occurences)
    {
        const auto most_common_bit = bit1_occurrence >= half_of_lines ? 1 : 0;
        bit1_occurrence            = most_common_bit;
    }

    const auto gamma_rate   = bits_array_to_number(bit1_occurences);
    const auto epsilon_rate = (~gamma_rate) & ((1 << *bits_width) - 1);

    return gamma_rate * epsilon_rate;
}

size_t solve03_b(std::istream& input)
{
    auto nums = std::ranges::istream_view<std::string>{ input } | std::ranges::to<std::vector<std::string>>();

    const auto oxygen_rating = filter_until_one(nums, '1');
    const auto co2_rating    = filter_until_one(nums, '0');

    return oxygen_rating * co2_rating;
}

}  // namespace year_2021
}  // namespace aoc
