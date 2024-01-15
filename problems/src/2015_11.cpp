#include "aoc/problems/2015_11.h"

#include <algorithm>
#include <cassert>
#include <istream>
#include <optional>
#include <ranges>

#include <boost/container/flat_set.hpp>

namespace
{
constexpr std::string_view prohibited = "iol";

char get_next_char(const char c)
{
    char next_char = c != 'z' ? c + 1 : 'a';
    if (prohibited.contains(next_char)) ++next_char;
    return next_char;
}

bool has_increasing_letters(const std::string& s)
{
    for (size_t i = 0; i < s.size() - 2; ++i)
    {
        if (s[ i ] == s[ i + 1 ] - 1 && s[ i ] == s[ i + 2 ] - 2) return true;
    }

    return false;
}

void inc_password(std::string& s)
{
    for (auto r_iter = s.rbegin(); r_iter != s.rend(); ++r_iter)
    {
        char& c = *r_iter;
        c       = get_next_char(c);

        if (c != 'a') return;
    }

    s.append(1, 'a');
}

bool is_valid_password(const std::string& password)
{
    if (!has_increasing_letters(password)) return false;

    boost::container::flat_set<char> pairs;

    char prev{};
    std::optional<char> prev_pair;

    for (const char c : password)
    {
        if (c == prev && !pairs.empty() && !pairs.contains(c)) return true;
        if (prev_pair)
        {
            pairs.insert(*prev_pair);
            prev_pair.reset();
        }
        if (c == prev) prev_pair = c;

        prev = c;
    }

    return false;
}

void next_non_prohibited(std::string& password)
{
    bool prohibited_matched = false;
    for (auto& c : password)
    {
        if (!prohibited_matched && prohibited.contains(c))
        {
            prohibited_matched = true;

            c = get_next_char(c);
        }
        else if (prohibited_matched) { c = 'a'; }
    }
}

std::string next_valid_password(std::string password)
{
    assert(std::ranges::all_of(password, islower));

    if (password.size() < 5) return "aabcc";

    next_non_prohibited(password);

    do {
        inc_password(password);
    } while (!is_valid_password(password));

    return password;
}
}  // namespace

namespace aoc
{
namespace year_2015
{

std::string solve11_a(std::istream& input)
{
    std::string old_password;
    input >> old_password;

    return next_valid_password(old_password);
}

std::string solve11_b(std::istream& input)
{
    return next_valid_password(solve11_a(input));
}

}  // namespace year_2015
}  // namespace aoc
