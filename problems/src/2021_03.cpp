#include "aoc/problems/2021_03.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace
{

std::string filter(std::vector<std::string> nums, size_t current_bit_idx, const bool most_common_criteria)
{
    assert(!nums.empty());

    while (nums.size() != 1)
    {
        size_t one_bits = 0;
        for (const auto& num : nums)
        {
            if (num[ current_bit_idx ] == '1')
                ++one_bits;
        }

        const size_t zero_bits = nums.size() - one_bits;
        const char bit_to_keep = ((one_bits >= zero_bits) == most_common_criteria) ? '1' : '0';

        nums.erase(std::remove_if(nums.begin(),
                                  nums.end(),
                                  [ current_bit_idx, bit_to_keep ](const auto& num) { return num[ current_bit_idx ] != bit_to_keep; }),
                   nums.end());

        ++current_bit_idx;
    }

    return nums.front();
}

std::string filter(const std::vector<std::string>& nums, const bool most_common_criteria)
{
    return filter(nums, 0, most_common_criteria);
}

}  // namespace

namespace aoc
{
namespace year_2021
{

size_t solve03_a(std::istream& input)
{
    std::string line;

    std::vector<size_t> bit_counters;

    size_t lines_total = 0;
    while (std::getline(input, line))
    {
        if (bit_counters.empty())
            bit_counters.resize(line.size());

        for (size_t i = 0; i < line.size(); ++i)
        {
            if (line[ i ] == '1')
                ++bit_counters[ i ];
        }

        ++lines_total;
    }

    const size_t half_lines = lines_total / 2;
    std::string bits(bit_counters.size(), '0');

    for (size_t i = 0; i < bits.size(); ++i)
    {
        assert(bit_counters[ i ] != half_lines);
        if (bit_counters[ i ] > half_lines)
            bits[ i ] = '1';
    }

    const size_t gamma_rate   = std::stoul(bits, nullptr, 2);
    const size_t epsilon_rate = ~gamma_rate & ((1 << bits.size()) - 1);

    return gamma_rate * epsilon_rate;
}

size_t solve03_b(std::istream& input)
{
    const auto nums = std::ranges::istream_view<std::string>(input) | std::ranges::to<std::vector>();

    const auto oxygen_generator_rating_bin = filter(nums, true);
    const auto CO2_scrubber_rating_bin     = filter(nums, false);

    const size_t oxygen_generator_rating = std::stoul(oxygen_generator_rating_bin, nullptr, 2);
    const size_t CO2_scrubber_rating     = std::stoul(CO2_scrubber_rating_bin, nullptr, 2);

    return oxygen_generator_rating * CO2_scrubber_rating;
}

}  // namespace year_2021
}  // namespace aoc