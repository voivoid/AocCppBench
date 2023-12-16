#pragma once

#include "boost/spirit/home/x3.hpp"
#include "boost/spirit/include/support_istream_iterator.hpp"
#include <boost/callable_traits/args.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>

#include <boost/iterator/function_input_iterator.hpp>
#include <boost/stl_interfaces/iterator_interface.hpp>

#include <istream>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>

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
bool x3_parse(std::string_view input, const Parser& parser, const Skipper& skipper)
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
bool x3_parse(std::string_view str, const Parser& parser, const Skipper& skipper, Attr& attr)
{
    return x3_parse(str.cbegin(), str.cend(), parser, skipper, attr);
}

template <typename Parser, typename Skipper, typename Attr>
bool x3_parse(std::istream& input, const Parser& parser, const Skipper& skipper, Attr& attr)
{
    input.unsetf(std::ios::skipws);
    return x3_parse(boost::spirit::istream_iterator{ input }, boost::spirit::istream_iterator{}, parser, skipper, attr);
}

template <typename Attr, typename Iter, typename Parser, typename Skipper>
std::optional<Attr> x3_parse_attr(Iter begin, Iter end, const Parser& parser, const Skipper& skipper)
{
    Attr attr;
    const bool parsed = boost::spirit::x3::phrase_parse(begin, end, parser, skipper, attr);
    return parsed && (begin == end) ? std::optional<Attr>{ std::move(attr) } : std::optional<Attr>{};
}

template <typename Attr, typename Parser, typename Skipper>
std::optional<Attr> x3_parse_attr(const std::string& str, const Parser& parser, const Skipper& skipper)
{
    return x3_parse_attr<Attr>(str.cbegin(), str.cend(), parser, skipper);
}

template <typename Attr, typename Parser, typename Skipper>
std::optional<Attr> x3_parse_attr(std::string_view str, const Parser& parser, const Skipper& skipper)
{
    return x3_parse_attr<Attr>(str.cbegin(), str.cend(), parser, skipper);
}

template <typename Attr, typename Parser, typename Skipper>
std::optional<Attr> x3_parse_attr(std::istream& input, const Parser& parser, const Skipper& skipper)
{
    input.unsetf(std::ios::skipws);
    return x3_parse_attr<Attr>(boost::spirit::istream_iterator{ input }, boost::spirit::istream_iterator{}, parser, skipper);
}

constexpr auto x3_size_t_ = boost::spirit::x3::ulong_;

template <typename Attr, typename Parser, typename Skipper>
struct parser_iterator
    : boost::stl_interfaces::iterator_interface<parser_iterator<Attr, Parser, Skipper>, std::input_iterator_tag, Attr>
{
    using iterator_concept = std::input_iterator_tag;
    using difference_type  = ptrdiff_t;

    parser_iterator() : m_input(nullptr), m_parser(nullptr), m_skipper(nullptr) {}

    parser_iterator(std::istream& input, const Parser& parser, const Skipper& skipper) :
        m_input(&input), m_parser(std::addressof(parser)), m_skipper(std::addressof(skipper))
    {
        parse_line();
    }

    const Attr& operator*() const noexcept
    {
        return m_attr;
    }

    parser_iterator& operator++() noexcept
    {
        parse_line();
        return *this;
    }

    parser_iterator operator++(int) noexcept
    {
        parser_iterator iter(*this);
        parse_line();
        return std::move(iter);
    }

    friend bool operator==(const parser_iterator& lhs, const parser_iterator& rhs) noexcept
    {
        return lhs.m_input == rhs.m_input;
    }

  private:
    void parse_line()
    {
        assert(m_input);
        assert(m_parser);
        assert(m_skipper);

        static constexpr size_t LineLen = 4096;
        char line[ LineLen ];
        if (!m_input->getline(line, sizeof(line)))
        {
            reset();
            return;
        }

        auto chars_read = m_input->gcount();
        if (chars_read == LineLen) throw std::runtime_error("buffer exceeded");

        if (line[ chars_read - 1 ] == 0) --chars_read;

        m_attr               = {};
        const bool is_parsed = aoc::x3_parse(std::begin(line), std::begin(line) + chars_read, *m_parser, *m_skipper, m_attr);
        if (!is_parsed) throw std::runtime_error("failed to parse");
    }

    void reset()
    {
        m_input = nullptr;
    }

  private:
    std::istream* m_input;
    const Parser* m_parser;
    const Skipper* m_skipper;

    Attr m_attr;
};

template <typename Attr, typename Parser, typename Skipper>
struct parser_range : std::ranges::view_interface<parser_range<Attr, Parser, Skipper>>
{
    parser_range(std::istream& input, const Parser& parser, const Skipper& skipper) :
        m_input(&input), m_parser(parser), m_skipper(skipper)
    {
    }

    parser_iterator<Attr, Parser, Skipper> begin() const
    {
        static_assert(std::input_iterator<parser_iterator<Attr, Parser, Skipper>>);
        return parser_iterator<Attr, Parser, Skipper>{ *m_input, m_parser, m_skipper };
    }

    parser_iterator<Attr, Parser, Skipper> end() const
    {
        return {};
    }

  private:
    std::istream* m_input;
    Parser m_parser;
    Skipper m_skipper;
};

template <typename Attr, typename Parser, typename Skipper>
parser_range<Attr, Parser, Skipper> parse_lines(std::istream& input, const Parser& parser, const Skipper& skipper)
{
    return parser_range<Attr, Parser, Skipper>(input, parser, skipper);
}

template <typename Attr, typename Parser>
auto parse_lines(std::istream& input, const Parser& parser)
{
    return parse_lines<Attr>(input, parser, boost::spirit::x3::space);
}

}  // namespace aoc
