#pragma once

#include "boost/spirit/home/x3.hpp"
#include "boost/spirit/include/support_istream_iterator.hpp"
#include <boost/callable_traits/args.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>

namespace aoc
{

template <typename Parser, typename Skipper>
bool x3_parse(const std::string& input, const Parser& parser, const Skipper& skipper)
{
    auto begin        = input.begin();
    auto end          = input.end();
    const bool parsed = boost::spirit::x3::phrase_parse(begin, end, parser, skipper);
    return parsed && (begin == end);
}

template <typename Parser, typename Skipper>
bool x3_parse(std::istream& input, const Parser& parser, const Skipper& skipper)
{
    input.unsetf(std::ios::skipws);
    auto begin        = boost::spirit::istream_iterator(input);
    auto end          = boost::spirit::istream_iterator{};
    const bool parsed = boost::spirit::x3::phrase_parse(begin, end, parser, skipper);
    return parsed && (begin == end);
}

template <typename Iter, typename Parser, typename Skipper, typename Attr>
bool x3_parse(Iter begin, Iter end, const Parser& parser, const Skipper& skipper, Attr& attr)
{
    const bool parsed = boost::spirit::x3::phrase_parse(begin, end, parser, skipper, attr);
    return parsed && (begin == end);
}

template <typename Parser, typename Skipper, typename Attr>
bool x3_parse(const std::string& str, const Parser& parser, const Skipper& skipper, Attr& attr)
{
    return x3_parse(str.cbegin(), str.cend(), parser, skipper, attr);
}

template <typename Parser, typename Skipper, typename Attr>
bool x3_parse(std::istream& input, const Parser& parser, const Skipper& skipper, Attr& attr)
{
    input.unsetf(std::ios::skipws);
    auto begin = boost::spirit::istream_iterator(input);
    auto end   = boost::spirit::istream_iterator{};
    return x3_parse(begin, end, parser, skipper, attr);
}

constexpr auto x3_size_t_ = boost::spirit::x3::ulong_;

template <typename Parser, typename Attr, size_t LineLen = 256>
bool try_parse_line(std::istream& input, const Parser& parser, Attr& attr)
{
    char line[ LineLen ];
    if (!input.getline(line, sizeof(line)))
    {
        return false;
    }

    auto chars_read = input.gcount();
    if (chars_read == LineLen)
        throw std::runtime_error("buffer exceeded");

    if (line[ chars_read - 1 ] == 0)
        --chars_read;

    const bool is_parsed = aoc::x3_parse(std::begin(line), std::begin(line) + chars_read, parser, boost::spirit::x3::space, attr);
    if (!is_parsed)
        throw std::runtime_error("failed to parse");

    return true;
}

template <typename Parser, typename F>
void parse_each_line_and_exec(std::istream& input, const Parser& parser, const F& handle_parsed_attr)
{
    using ParsedAttr = std::remove_cv_t<std::remove_reference_t<std::tuple_element_t<0, boost::callable_traits::args_t<F>>>>;

    ParsedAttr attr;
    while (try_parse_line(input, parser, attr))
    {
        handle_parsed_attr(attr);
    }
}

}  // namespace aoc