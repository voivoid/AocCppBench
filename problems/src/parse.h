#pragma once

#include "boost/spirit/home/x3.hpp"
#include "boost/spirit/include/support_istream_iterator.hpp"
#include <boost/callable_traits/args.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>

#include <boost/iterator/function_input_iterator.hpp>

namespace aoc
{

template <typename Iter, typename Parser, typename Skipper>
bool x3_parse(Iter begin, Iter end, const Parser& parser, const Skipper& skipper)
{
    const bool parsed = boost::spirit::x3::phrase_parse(begin, end, parser, skipper);
    return parsed && (begin == end);
}

template <typename Parser, typename Skipper>
bool x3_parse(const std::string& input, const Parser& parser, const Skipper& skipper)
{
    return x3_parse(input.cbegin(), input.cend(), parser, skipper);
}

template <typename Parser, typename Skipper>
bool x3_parse(std::istream& input, const Parser& parser, const Skipper& skipper)
{
    input.unsetf(std::ios::skipws);
    return x3_parse(boost::spirit::istream_iterator{ input }, boost::spirit::istream_iterator{}, parser, skipper);
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
    return x3_parse(boost::spirit::istream_iterator{ input }, boost::spirit::istream_iterator{}, parser, skipper, attr);
}

constexpr auto x3_size_t_ = boost::spirit::x3::ulong_;

template <typename Parser, typename F>
void parse_each_line_and_exec(std::istream& input, const Parser& parser, const F& handle_parsed_attr)
{
    static constexpr size_t LineLen = 1024;
    using ParsedAttr                = std::remove_cv_t<std::remove_reference_t<std::tuple_element_t<0, boost::callable_traits::args_t<F>>>>;

    char line[ LineLen ];
    while (input.getline(line, sizeof(line)))
    {
        auto chars_read = input.gcount();
        if (chars_read == LineLen)
            throw std::runtime_error("buffer exceeded");

        if (line[ chars_read - 1 ] == 0)
            --chars_read;

        ParsedAttr attr;
        const bool is_parsed = aoc::x3_parse(std::begin(line), std::begin(line) + chars_read, parser, boost::spirit::x3::space, attr);
        if (!is_parsed)
            throw std::runtime_error("failed to parse");

        handle_parsed_attr(attr);
    }
}

}  // namespace aoc