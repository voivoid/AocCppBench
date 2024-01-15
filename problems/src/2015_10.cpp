#include "aoc/problems/2015_10.h"

#include <cassert>
#include <istream>
#include <ranges>
#include <string>

namespace
{
void look_and_say(std::string& current_value, std::string& next_value)
{
    assert(!current_value.empty());

    next_value.clear();

    size_t chunk_size = 0;
    char chunk_char   = current_value.front();

    const auto add_chunk = [ & ]()
    {
        next_value.push_back(static_cast<char>('0' + chunk_size));
        next_value.push_back(chunk_char);
    };

    for (const char c : current_value)
    {
        if (c == chunk_char) { ++chunk_size; }
        else
        {
            add_chunk();
            chunk_char = c;
            chunk_size = 1;
        }
    }
    add_chunk();

    current_value.swap(next_value);
}
}  // namespace

namespace aoc
{
namespace year_2015
{
namespace details
{

size_t solve10(std::istream& input, size_t steps)
{
    std::string s, buffer;

    const auto estimated_reserve = steps > 40 ? 4000000 : steps > 10 ? 400000 : 40;
    s.reserve(estimated_reserve);
    buffer.reserve(estimated_reserve);

    input >> s;

    while (steps--) { look_and_say(s, buffer); }

    return s.size();
}

}  // namespace details

size_t solve10_a(std::istream& input)
{
    return details::solve10(input, 40);
}

size_t solve10_b(std::istream& input)
{
    return details::solve10(input, 50);
}

}  // namespace year_2015
}  // namespace aoc
