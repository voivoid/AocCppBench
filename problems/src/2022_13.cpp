#include "aoc/problems/2022_13.h"

#include "parse.h"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <compare>
#include <iostream>
#include <variant>
#include <vector>
#include <algorithm>

namespace
{
namespace ast
{

namespace x3 = boost::spirit::x3;

struct expr;

struct expr_value : x3::variant<size_t, x3::forward_ast<expr>>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct expr
{
    std::vector<expr_value> values;
};

struct check_ordering
{
    std::strong_ordering operator()(const expr& e1, const expr& e2) const
    {
        const auto& e1vs = e1.values;
        const auto& e2vs = e2.values;

        const auto max_elems = std::max(e1vs.size(), e2vs.size());
        for (size_t i = 0; i < max_elems; ++i)
        {
            if (i == e1vs.size())
                return std::strong_ordering::less;
            if (i == e2vs.size())
                return std::strong_ordering::greater;

            const expr_value& v1 = e1vs[ i ];
            const expr_value& v2 = e2vs[ i ];

            const auto order = boost::apply_visitor(*this, v1, v2);
            if (order == std::strong_ordering::equal)
                continue;

            return order;
        }

        return std::strong_ordering::equal;
    }

    std::strong_ordering operator()(const size_t v1, const expr& e2) const
    {
        const auto e1 = expr{ { expr_value{ v1 } } };
        return (*this)(e1, e2);
    }

    std::strong_ordering operator()(const expr& e1, const size_t v2) const
    {
        const auto e2 = expr{ { expr_value{ v2 } } };
        return (*this)(e1, e2);
    }

    std::strong_ordering operator()(const size_t v1, const size_t v2) const
    {
        return v1 <=> v2;
    }
};

bool operator<(const expr& e1, const expr& e2)
{
    return check_ordering{}(e1, e2) == std::strong_ordering::less;
}

bool operator==(const expr& e1, const expr& e2)
{
    return check_ordering{}(e1, e2) == std::strong_ordering::equal;
}

}  // namespace ast
}  // namespace

BOOST_FUSION_ADAPT_STRUCT(ast::expr, values)

namespace
{
namespace parser
{

namespace x3 = boost::spirit::x3;

const x3::rule<struct _expr_value, ast::expr_value> expr_value = "expr_value";
const x3::rule<struct _expr, ast::expr> expr                   = "expr";
const x3::rule<struct _exprs, std::vector<ast::expr>> exprs    = "exprs";

const auto expr_value_def = aoc::x3_size_t_ | expr;
const auto expr_def       = x3::lit('[') > ((expr_value % ',') | x3::eps) > x3::lit(']');
const auto exprs_def      = +expr;

BOOST_SPIRIT_DEFINE(expr_value, expr, exprs)

}  // namespace parser

ast::expr parse_expr(const std::string& line)
{
    ast::expr e;
    bool parsed = aoc::x3_parse(line, parser::expr, boost::spirit::x3::space, e);

    if (!parsed)
        throw std::runtime_error("failed to parse expr");

    return e;
}

}  // namespace


namespace aoc
{
namespace year_2022
{

size_t solve13_a(std::istream& input)
{
    size_t right_order_indices_sum  = 0;
    size_t idx                      = 1;

    std::string line1;
    std::string line2;
    while ((input >> line1) && (input >> line2))
    {
        const ast::expr e1 = parse_expr(line1);
        const ast::expr e2 = parse_expr(line2);

        const auto ordering = ast::check_ordering{}(e1, e2);
        if (ordering == std::strong_ordering::less)
            right_order_indices_sum += idx;

        ++idx;
    }

    return right_order_indices_sum;
}

size_t solve13_b(std::istream& input)
{
    std::vector<ast::expr> exprs;
    if (!aoc::x3_parse(input, parser::exprs, boost::spirit::x3::space, exprs))
        throw std::runtime_error("failed to parse exprs");

    const auto div1 = parse_expr("[[2]]");
    const auto div2 = parse_expr("[[6]]");

    exprs.push_back(div1);
    exprs.push_back(div2);

    std::sort(exprs.begin(), exprs.end());

    auto div1_iter = std::find(exprs.cbegin(), exprs.cend(), div1);    
    auto div2_iter = std::find(exprs.cbegin(), exprs.cend(), div2);

    assert(div1_iter != exprs.cend());
    assert(div2_iter != exprs.cend());
    
    const auto div1_idx = std::distance(exprs.cbegin(), div1_iter) + 1;
    const auto div2_idx = std::distance(exprs.cbegin(), div2_iter) + 1;

    return div1_idx * div2_idx;
}

}  // namespace year_2022
}  // namespace aoc