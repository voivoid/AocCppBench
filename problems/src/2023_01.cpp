#include "aoc/problems/2023_01.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <istream>
#include <ranges>

namespace
{

enum class digits_parser_mode
{
    digits_only,
    digits_and_words_direct,
    digits_and_words_reverse
};

using Digit = size_t;

boost::spirit::x3::symbols<Digit> make_digits_map(const digits_parser_mode mode)
{
    static constexpr const char* digit_words[]{
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };

    boost::spirit::x3::symbols<Digit> symbols_map{ { "1", 1 }, { "2", 2 }, { "3", 3 }, { "4", 4 }, { "5", 5 },
                                                   { "6", 6 }, { "7", 7 }, { "8", 8 }, { "9", 9 } };

    if (mode != digits_parser_mode::digits_only)
    {
        for (size_t i = 0; i < std::size(digit_words); ++i)
        {
            std::string word = digit_words[ i ];
            if (mode == digits_parser_mode::digits_and_words_reverse) { std::reverse(word.begin(), word.end()); }
            symbols_map.add(word, i + 1);
        }
    }


    return symbols_map;
}

auto make_parser(const digits_parser_mode mode)
{
    namespace x3 = boost::spirit::x3;

    const auto digits_map = make_digits_map(mode);

    return x3::omit[ *(x3::char_ - digits_map) ] > digits_map > x3::omit[ *(x3::char_) ];
}

auto make_digits_parser()
{
    return make_parser(digits_parser_mode::digits_only);
}

auto make_digits_and_words_parsers()
{
    return std::make_pair(make_parser(digits_parser_mode::digits_and_words_direct),
                          make_parser(digits_parser_mode::digits_and_words_reverse));
};

template <typename Parser>
size_t calc_extrapolation(std::string_view input_line, const Parser& direct_parser, const Parser& reverse_parser)
{
    auto digit1 =
        aoc::x3_parse_attr<Digit>(input_line.cbegin(), input_line.cend(), direct_parser, boost::spirit::x3::space);
    ensure(digit1);

    auto digit2 =
        aoc::x3_parse_attr<Digit>(input_line.crbegin(), input_line.crend(), reverse_parser, boost::spirit::x3::space);
    ensure(digit2);

    assert(*digit1 <= 9);
    assert(*digit2 <= 9);

    return *digit1 * 10 + *digit2;
}

template <typename Parser>
size_t solve(std::istream& input, const Parser& direct_parser, const Parser& reverse_parser)
{
    auto calc_line_f = std::bind_back(&calc_extrapolation<Parser>, std::ref(direct_parser), std::ref(reverse_parser));
    auto input_lines = std::ranges::views::istream<std::string>(input);
    const auto sum =
        std::ranges::fold_left(input_lines | std::ranges::views::transform(calc_line_f), size_t(0), std::plus{});

    return sum;
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve01_a(std::istream& input)
{
    const auto digits_parser = make_digits_parser();
    return solve(input, digits_parser, digits_parser);
}

size_t solve01_b(std::istream& input)
{
    const auto [ words_direct_parser, words_reverse_parser ] = make_digits_and_words_parsers();
    return solve(input, words_direct_parser, words_reverse_parser);
}

}  // namespace year_2023
}  // namespace aoc