#include "aoc/problems/2022_03.h"

#include "ensure.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>

namespace
{
char find_item(const std::string& line)
{
    const size_t str_len      = line.size();
    const size_t half_str_len = str_len / 2;

    std::unordered_set<char> left_compartment;
    for (size_t i = 0; i < half_str_len; ++i)
    {
        left_compartment.insert(line[ i ]);
    }


    for (size_t i = half_str_len; i < str_len; ++i)
    {
        auto item = line[ i ];
        if (left_compartment.contains(item))
            return item;
    }

    throw std::runtime_error("no items found");
}

size_t get_item_priority(const char item)
{
    if (std::islower(item))
    {
        return item - 'a' + 1;
    }
    else
    {
        assert(std::isupper(item));
        return item - 'A' + 1 + 26;
    }
}

}  // namespace

namespace aoc
{
namespace year_2022
{

size_t solve03_a(std::istream& input)
{
    size_t priorities_sum = 0;
    std::string line;

    while (std::getline(input, line))
    {
        const char item = find_item(line);
        priorities_sum += get_item_priority(item);
    }

    return priorities_sum;
}

size_t solve03_b(std::istream& input)
{
    size_t priorities_sum = 0;
    std::string line;

    while (std::getline(input, line))
    {
        const std::unordered_set<char> s1(line.cbegin(), line.cend());
        ensure(std::getline(input, line));
        const std::unordered_set<char> s2(line.cbegin(), line.cend());
        ensure(std::getline(input, line));

        auto badge_iter =
            std::find_if(line.cbegin(), line.cend(), [ &s1, &s2 ](const char item) { return s1.contains(item) && s2.contains(item); });
        assert(badge_iter != line.cend());

        priorities_sum += get_item_priority(*badge_iter);
    }

    return priorities_sum;
}

}  // namespace year_2022
}  // namespace aoc