#include "aoc/problems/2023_01.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <istream>
#include <ranges>
#include <string>

namespace
{

enum class digits_parser_mode
{
    direct,
    reverse
};

using Digit = size_t;

boost::spirit::x3::symbols<Digit> make_digits_map(const digits_parser_mode mode)
{
    static constexpr const char* digit_words[]{
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };

    boost::spirit::x3::symbols<Digit> symbols_map{ { "1", 1 }, { "2", 2 }, { "3", 3 }, { "4", 4 }, { "5", 5 },
                                                   { "6", 6 }, { "7", 7 }, { "8", 8 }, { "9", 9 } };

    for (size_t i = 0; i < std::size(digit_words); ++i)
    {
        std::string word = digit_words[ i ];
        if (mode == digits_parser_mode::reverse) { std::reverse(word.begin(), word.end()); }
        symbols_map.add(word, i + 1);
    }

    return symbols_map;
}

auto make_parser(const digits_parser_mode mode)
{
    namespace x3 = boost::spirit::x3;

    const auto digits_map = make_digits_map(mode);
    return x3::omit[ *(x3::char_ - digits_map) ] > digits_map;
}

template <typename Parser>
size_t calc_extrapolation(const std::string& input_line, const Parser& direct_parser, const Parser& reverse_parser)
{
    auto digit1 = aoc::x3_parse_attr<Digit>(
        input_line.cbegin(), input_line.cend(), direct_parser, boost::spirit::x3::space, false);
    ensure(digit1);

    auto digit2 = aoc::x3_parse_attr<Digit>(
        input_line.crbegin(), input_line.crend(), reverse_parser, boost::spirit::x3::space, false);
    ensure(digit2);

    assert(*digit1 <= 9);
    assert(*digit2 <= 9);

    return *digit1 * 10 + *digit2;
}

size_t calc_calibration_value(const std::string& s)
{
    static const char* const digits = "0123456789";

    const auto first_digit_idx = s.find_first_of(digits);
    const auto last_digit_idx  = s.find_last_of(digits);
    assert(first_digit_idx != std::string::npos);
    assert(last_digit_idx != std::string::npos);

    const auto first_digit = s[ first_digit_idx ];
    const auto last_digit  = s[ last_digit_idx ];
    assert(std::isdigit(first_digit));
    assert(std::isdigit(last_digit));

    return (first_digit - '0') * 10 + (last_digit - '0');
}

template <typename F>
size_t solve(std::istream& input, const F& calc_values)
{
    using namespace std::ranges;

    auto values    = views::istream<std::string>(input) | views::transform(calc_values);
    const auto sum = fold_left(values, size_t(0), std::plus{});
    return sum;
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve01_a(std::istream& input)
{
    return solve(input, calc_calibration_value);
}

size_t solve01_b(std::istream& input)
{
    const auto direct_parser  = make_parser(digits_parser_mode::direct);
    const auto reverse_parser = make_parser(digits_parser_mode::reverse);

    return solve(input,
                 [ & ](const std::string& line) { return calc_extrapolation(line, direct_parser, reverse_parser); });
}

}  // namespace year_2023
}  // namespace aoc