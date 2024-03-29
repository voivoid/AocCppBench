#pragma once

#include "fusion.h"

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4459)
#endif

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

#include <cassert>
#include <istream>
#include <iterator>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>

namespace aoc
{

// no attr, no skipper

template <typename Iter, typename Parser>
bool x3_parse(Iter begin, Iter end, const Parser& parser)
{
    const bool parsed = boost::spirit::x3::parse(begin, end, parser);
    return parsed && (begin == end);
}

template <typename Parser>
bool x3_parse(const std::string& input, const Parser& parser)
{
    return x3_parse(input.cbegin(), input.cend(), parser);
}

template <typename Parser>
bool x3_parse(std::string_view input, const Parser& parser)
{
    return x3_parse(input.cbegin(), input.cend(), parser);
}

template <typename Parser>
bool x3_parse(std::istream& input, const Parser& parser)
{
    input.unsetf(std::ios::skipws);
    return x3_parse(boost::spirit::istream_iterator{ input }, boost::spirit::istream_iterator{}, parser);
}

// no attr, with skipper

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

// out attr, with skipper

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

// ret attr, with skipper

template <typename Attr, typename Parser, typename Skipper>
std::optional<Attr> x3_parse_attr(boost::spirit::istream_iterator& begin, const Parser& parser, const Skipper& skipper)
{
    Attr attr;
    const bool parsed =
        boost::spirit::x3::phrase_parse(begin, boost::spirit::istream_iterator{}, parser, skipper, attr);
    return parsed ? std::optional<Attr>{ std::move(attr) } : std::optional<Attr>{};
}

template <typename Attr, typename Iter, typename Parser, typename Skipper>
std::optional<Attr>
    x3_parse_attr(Iter begin, Iter end, const Parser& parser, const Skipper& skipper, bool parse_all = true)
{
    Attr attr;
    const bool parsed = boost::spirit::x3::phrase_parse(begin, end, parser, skipper, attr);
    return parsed && (!parse_all || (begin == end)) ? std::optional<Attr>{ std::move(attr) } : std::optional<Attr>{};
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
    return x3_parse_attr<Attr>(
        boost::spirit::istream_iterator{ input }, boost::spirit::istream_iterator{}, parser, skipper);
}

constexpr auto x3_size_t_ = boost::spirit::x3::ulong_long;

template <typename Attr, typename Parser, typename Skipper, char separator>
struct parser_iterator
{
    using value_type        = Attr;
    using reference         = Attr&;
    using pointer           = Attr*;
    using difference_type   = ptrdiff_t;
    using iterator_category = std::input_iterator_tag;
    using iterator_concept  = std::input_iterator_tag;

    parser_iterator() : m_input(nullptr), m_parser(nullptr), m_skipper(nullptr) {}

    parser_iterator(std::istream& input, const Parser& parser, const Skipper& skipper) :
        m_input(&input), m_parser(std::addressof(parser)), m_skipper(std::addressof(skipper))
    {
        parse_line();
    }

    reference operator*() noexcept
    {
        return m_attr;
    }

    reference operator*() const noexcept
    {
        return const_cast<Attr&>(m_attr);
    }

    auto operator->() const noexcept
    {
        return &m_attr;
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
        if (!m_input->getline(line, sizeof(line), separator))
        {
            reset();
            return;
        }

        auto chars_read = m_input->gcount();
        if (chars_read == LineLen) throw std::runtime_error("buffer exceeded");

        if (line[ chars_read - 1 ] == 0) --chars_read;

        m_attr = {};
        const bool is_parsed =
            aoc::x3_parse(std::begin(line), std::begin(line) + chars_read, *m_parser, *m_skipper, m_attr);
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

template <typename Attr, char separator, typename Parser, typename Skipper>
struct parser_range : std::ranges::view_interface<parser_range<Attr, separator, Parser, Skipper>>
{
    parser_range(std::istream& input, const Parser& parser, const Skipper& skipper) :
        m_input(&input), m_parser(parser), m_skipper(skipper)
    {
    }

    parser_iterator<Attr, Parser, Skipper, separator> begin() const
    {
        static_assert(std::input_iterator<parser_iterator<Attr, Parser, Skipper, separator>>);
        return parser_iterator<Attr, Parser, Skipper, separator>{ *m_input, m_parser, m_skipper };
    }

    parser_iterator<Attr, Parser, Skipper, separator> end() const
    {
        return {};
    }

  private:
    std::istream* m_input;
    Parser m_parser;
    Skipper m_skipper;
};

template <typename Attr, char separator = '\n', typename Parser, typename Skipper>
parser_range<Attr, separator, Parser, Skipper>
    parse_lines(std::istream& input, const Parser& parser, const Skipper& skipper)
{
    return parser_range<Attr, separator, Parser, Skipper>(input, parser, skipper);
}

template <typename Attr, char separator = '\n', typename Parser>
auto parse_lines(std::istream& input, const Parser& parser)
{
    return parse_lines<Attr, separator>(input, parser, boost::spirit::x3::space);
}

auto opt_separated_list(const auto& left, const auto& right)
{
    return *(left >> -right);
}

auto x3_attrs_tuple(const auto& ctx)
{
    return aoc::fusion_to_std_tuple(boost::spirit::x3::_attr(ctx));
}

}  // namespace aoc
