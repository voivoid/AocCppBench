#include "aoc/problems/2023_19.h"

#include "ensure.h"
#include "parse.h"

#include <istream>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/icl/closed_interval.hpp>

namespace
{
using interval = boost::icl::closed_interval<size_t>;

struct rule
{
    char part;
    char cmp_op;
    size_t comparable;
    std::string destination;
};

struct workflow
{
    std::vector<rule> rules;
    std::string last_rule_dest;
};

using workflow_map = std::unordered_map<std::string, workflow>;

const std::string start_workflow_id = "in";
const std::string accept_dest       = "A";
const std::string reject_dest       = "R";
const std::string part_names        = "xmas";
constexpr auto min_rating           = 1;
constexpr auto max_rating           = 4000;

struct parts
{
    static size_t part_idx(const char c)
    {
        switch (c)
        {
            case 'x': return 0;
            case 'm': return 1;
            case 'a': return 2;
            case 's': return 3;
        }

        std::unreachable();
    }

    size_t get_value(const char c) const
    {
        return values[ part_idx(c) ];
    }

    size_t values[ 4 ] = {};
};
}  // namespace

BOOST_FUSION_ADAPT_STRUCT(rule, part, cmp_op, comparable, destination)
BOOST_FUSION_ADAPT_STRUCT(workflow, rules, last_rule_dest)
BOOST_FUSION_ADAPT_STRUCT(parts, values)

namespace
{

workflow_map parse_workflow_map(boost::spirit::istream_iterator& iter)
{
    namespace x3 = boost::spirit::x3;

    const auto workflow_name_parser = +x3::alpha;

    const auto rule_parser = x3::rule<struct _rule, rule>{} =
        x3::char_(part_names) >> x3::char_("><") > aoc::x3_size_t_ > x3::lit(':') > workflow_name_parser;

    const auto workflow_parser = x3::rule<struct _workflow, workflow>{} =
        (rule_parser % ',') > x3::lit(',') > workflow_name_parser;

    const auto workflow_map_parser = *(workflow_name_parser > x3::lit('{') > workflow_parser > x3::lit('}'));

    auto workflow_map = aoc::x3_parse_attr<::workflow_map>(iter, workflow_map_parser, x3::space);
    ensure(workflow_map);

    return *workflow_map;
}

std::optional<parts> parse_parts(boost::spirit::istream_iterator& iter)
{
    namespace x3 = boost::spirit::x3;

    const auto part_action = [](const auto& ctx)
    {
        const auto [ part, value ] = aoc::x3_attrs_tuple(ctx);

        x3::_val(ctx).values[ parts::part_idx(part) ] = value;
    };

    const auto part_parser  = x3::char_(part_names) > x3::lit('=') > aoc::x3_size_t_;
    const auto parts_parser = x3::rule<struct _parts, parts>{} =
        x3::lit('{') > (part_parser[ part_action ] % ',') > x3::lit('}');

    return aoc::x3_parse_attr<parts>(iter, parts_parser, x3::space);
}

bool run_workflows(const workflow_map& map, const workflow* current_workflow, const parts& parts)
{
    assert(current_workflow);

    for (;;)
    {
        const std::string* destination = nullptr;
        for (const auto& rule : current_workflow->rules)
        {
            const auto part_value = parts.get_value(rule.part);
            const bool goto_dest  = (rule.cmp_op == '>' && part_value > rule.comparable) ||
                                   (rule.cmp_op == '<' && part_value < rule.comparable);

            if (goto_dest)
            {
                destination = &rule.destination;
                break;
            }
        }

        if (!destination) destination = &current_workflow->last_rule_dest;

        if (*destination == accept_dest) return true;
        if (*destination == reject_dest) return false;

        auto workflow_iter = map.find(*destination);
        assert(workflow_iter != map.cend());
        current_workflow = &workflow_iter->second;
    }
}

size_t sum_parts(const parts& parts)
{
    return std::ranges::fold_left(parts.values, size_t(0), std::plus{});
}

struct traversal_state
{
    void intersect_with(const char part, const interval& interval)
    {
        const auto idx   = parts::part_idx(part);
        intervals[ idx ] = intervals[ idx ] & interval;
    }

    interval intervals[ 4 ];
};

size_t mul_intervals(const traversal_state& state)
{
    size_t sum = 1;
    for (const auto& interval : state.intervals) { sum *= interval.upper() - interval.lower() + 1; }
    return sum;
}

traversal_state next_state_with_rule_applied(traversal_state state, const rule& rule)
{
    const interval new_interval =
        rule.cmp_op == '<' ? interval(min_rating, rule.comparable - 1) : interval(rule.comparable + 1, max_rating);
    state.intersect_with(rule.part, new_interval);
    return state;
}

void apply_opposite_rule(traversal_state& state, const rule& rule)
{
    const interval new_interval =
        rule.cmp_op == '<' ? interval(rule.comparable, max_rating) : interval(min_rating, rule.comparable);
    state.intersect_with(rule.part, new_interval);
}

size_t calc_combinations(const workflow_map& workflow_map, const std::string& workflow_id, traversal_state state)
{
    if (workflow_id == reject_dest) return 0;
    if (workflow_id == accept_dest) return mul_intervals(state);

    auto workflow_iter = workflow_map.find(workflow_id);
    assert(workflow_iter != workflow_map.cend());
    const auto& workflow = workflow_iter->second;

    size_t sum = 0;
    for (const auto& rule : workflow.rules)
    {
        sum += calc_combinations(workflow_map, rule.destination, next_state_with_rule_applied(state, rule));
        apply_opposite_rule(state, rule);
    }

    sum += calc_combinations(workflow_map, workflow.last_rule_dest, state);

    return sum;
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve19_a(std::istream& input)
{
    boost::spirit::istream_iterator istream_iter(input);
    const auto workflow_map = parse_workflow_map(istream_iter);

    auto in_workflow_iter = workflow_map.find(start_workflow_id);
    assert(in_workflow_iter != workflow_map.cend());

    const auto& in_workflow = in_workflow_iter->second;

    size_t sum = 0;
    while (auto parts = parse_parts(istream_iter))
    {
        if (run_workflows(workflow_map, &in_workflow, *parts)) { sum += sum_parts(*parts); }
    }

    assert(sum >= 0);
    return static_cast<size_t>(sum);
}

size_t solve19_b(std::istream& input)
{
    const interval full_interval{ min_rating, max_rating };
    const traversal_state initial_state{ full_interval, full_interval, full_interval, full_interval };

    boost::spirit::istream_iterator istream_iter(input);
    const auto workflow_map = parse_workflow_map(istream_iter);

    return calc_combinations(workflow_map, start_workflow_id, initial_state);
}

}  // namespace year_2023
}  // namespace aoc
