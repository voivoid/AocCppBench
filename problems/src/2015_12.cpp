#include "aoc/problems/2015_12.h"

#include "ensure.h"
#include "parse.h"

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/variant.hpp>

#include <ranges>
#include <unordered_map>

namespace
{
using num = int;
using str = std::string;

namespace x3 = boost::spirit::x3;

using val = boost::make_recursive_variant<num,
                                          str,
                                          std::vector<boost::recursive_variant_>,
                                          std::unordered_map<str, boost::recursive_variant_>>::type;
using arr = std::vector<val>;
using obj = std::unordered_map<str, val>;

namespace x3 = boost::spirit::x3;

val parse_json_value(std::istream& input)
{
    x3::rule<struct _val, ::val> value;

    const auto num           = x3::int_;
    const auto str           = x3::lit('\"') > (+x3::alpha) > x3::lit('\"');
    const auto array         = x3::lit('[') > aoc::opt_separated_list(value, x3::lit(',')) > x3::lit(']');
    const auto key_value     = str > x3::lit(':') > value;
    const auto object_values = x3::rule<struct _obj, obj>{} = aoc::opt_separated_list(key_value, x3::lit(','));
    const auto object                                       = (x3::lit('{') > object_values > x3::lit('}'));

    const auto value_parser = value = object | array | str | num;

    auto val_opt = aoc::x3_parse_attr<::val>(input, value_parser, boost::spirit::x3::space);
    ensure(val_opt);

    return std::move(val_opt).value();
}

template <bool ignore_red>
struct sum_numbers : public boost::static_visitor<size_t>
{
    size_t operator()(num n) const
    {
        return n;
    }

    size_t operator()(const str&) const
    {
        return 0;
    }

    size_t operator()(const arr& arr) const
    {
        size_t sum = 0;
        for (const auto& val : arr) { sum += boost::apply_visitor(*this, val); }

        return sum;
    }

    size_t operator()(const obj& obj) const
    {
        size_t sum = 0;
        for (const auto& [ key, val ] : obj)
        {
            if constexpr (ignore_red)
            {
                if (const str* val_str = boost::get<str>(&val); val_str && *val_str == "red")
                {
                    sum = 0;
                    break;
                }
            }
            sum += boost::apply_visitor(*this, val);
        }

        return sum;
    }

    size_t operator()(const val& val) const
    {
        return boost::apply_visitor(*this, val);
    }
};


template <bool ignore_red>
size_t solve(std::istream& input)
{
    const val json   = parse_json_value(input);
    const size_t sum = boost::apply_visitor(sum_numbers<ignore_red>(), json);

    return sum;
}
}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve12_a(std::istream& input)
{
    return solve<false>(input);
}

size_t solve12_b(std::istream& input)
{
    return solve<true>(input);
}

}  // namespace year_2015
}  // namespace aoc
