#include "aoc/problems/2015_08.h"

#include "ensure.h"
#include "parse.h"
#include "stream.h"

#include <istream>
#include <ranges>

namespace aoc
{
namespace year_2015
{

size_t solve08_a(std::istream& input)
{
    namespace x3 = boost::spirit::x3;

    size_t in_memory = 0;
    size_t chars     = 0;

    const auto inc_in_memory = [ &in_memory ](const auto&) { ++in_memory; };
    const auto inc_chars     = [ & ](const size_t n) { return [ n, &chars ](const auto&) { chars += n; }; };

    const auto chr       = x3::alnum;
    const auto quote     = x3::lit("\\\"");
    const auto backslash = x3::lit("\\\\");
    const auto hex_chr   = x3::lit("\\x") >> x3::xdigit >> x3::xdigit;
    const auto symbol =
        (hex_chr[ inc_chars(4) ] | (backslash | quote)[ inc_chars(2) ] | chr[ inc_chars(1) ])[ inc_in_memory ];
    const auto line   = (x3::lit('\"') >> *symbol >> x3::lit('\"'))[ inc_chars(2) ];
    const auto parser = *x3::lexeme[ line ];

    ensure(aoc::x3_parse(input, parser, x3::space));

    return chars - in_memory;
}

size_t solve08_b(std::istream& input)
{
    size_t diff = 0;

    for (const std::string& line : std::ranges::views::istream<aoc::lines<>>(input))
    {
        assert(!line.empty());

        diff += 2;
        for (const char c : line)
        {
            if (c == '\"' || c == '\\') ++diff;
        }
    };

    return diff;
}

}  // namespace year_2015
}  // namespace aoc
