#include "aoc/problems/2015_05.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <istream>
#include <ranges>
#include <string>
#include <unordered_set>

namespace
{
bool is_vowel(const char c)
{
    assert(std::isalpha(c));

    static constexpr std::string_view vowels("aeiou");
    return vowels.find(c) != std::string::npos;
}

bool has_twice_in_a_row_letter(const std::string& str)
{
    const auto pairs = str | std::ranges::views::pairwise_transform(std::equal_to{});

    return std::ranges::any_of(pairs, std::identity{});
}

bool has_no_prohibited_strings(const std::string& str)
{
    static constexpr std::string_view prohibited_strings[] = { "ab", "cd", "pq", "xy" };

    return std::ranges::all_of(prohibited_strings,
                               [ &str ](const auto& prohibited) { return !str.contains(prohibited); });
}

bool has_pair_of_letters(const std::string& str)
{
    const auto pair_strs =
        str | std::ranges::views::slide(2) |
        std::ranges::views::transform([](const auto pair) { return std::string(pair.cbegin(), pair.cend()); });

    std::unordered_set<std::string> visited_pairs;

    std::string next;
    for (const auto pair_str : pair_strs)
    {
        if (visited_pairs.contains(pair_str)) return true;

        visited_pairs.insert(next);
        next = pair_str;
    }

    return false;
}

bool has_inbetween_letter(const std::string& str)
{
    const auto nice_triples = str | std::ranges::views::adjacent_transform<3>(
                                        [](const char c1, const char, const char c3) { return c1 == c3; });

    return std::ranges::any_of(nice_triples, std::identity{});
}

bool is_nice1(const std::string& str)
{
    const auto vowels = std::ranges::count_if(str, is_vowel);

    return vowels >= 3 && has_twice_in_a_row_letter(str) && has_no_prohibited_strings(str);
}

bool is_nice2(const std::string& str)
{
    return has_inbetween_letter(str) && has_pair_of_letters(str);
}

template <auto is_nice>
size_t solve(std::istream& input)
{
    auto strs = std::ranges::views::istream<std::string>(input);
    return std::ranges::count_if(strs, is_nice);
}
}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve05_a(std::istream& input)
{
    return solve<is_nice1>(input);
}

size_t solve05_b(std::istream& input)
{
    return solve<is_nice2>(input);
}

}  // namespace year_2015
}  // namespace aoc
