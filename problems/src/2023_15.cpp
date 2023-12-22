#include "aoc/problems/2023_15.h"

#include "parse.h"
#include "ensure.h"

#include <istream>
#include <string>
#include <ranges>
#include <algorithm>
#include <variant>
#include <map>

#include <boost/fusion/adapted/struct.hpp>
#include <boost/functional/overloaded_function.hpp>
#include <boost/variant.hpp>

namespace
{
    struct lens
    {
        std::string label;
        size_t focal_length;
    };

    struct remove_lens
    {
        std::string label;
    };

    using box = std::vector<lens>;
    using box_id = std::uint8_t;
    struct state
    {
        std::map<box_id, box> box_map;
    };

    using add_lens = lens;

}
BOOST_FUSION_ADAPT_STRUCT(lens, label, focal_length);
BOOST_FUSION_ADAPT_STRUCT(remove_lens, label);

namespace
{

    using command = boost::variant<remove_lens, add_lens>;

    command parse_command(const std::string& str)
    {
        namespace x3 = boost::spirit::x3;

        const auto label_parser = +x3::alpha;

        const auto remove_lens_parser = x3::rule<struct _remove, remove_lens>{} = label_parser >> x3::lit('-');
        const auto add_lens_parser = x3::rule<struct _add, add_lens>{} = label_parser >> x3::lit('=') > aoc::x3_size_t_;
        const auto parser = x3::rule<struct _p, command>{} = add_lens_parser | remove_lens_parser;

        auto cmd = aoc::x3_parse_attr<command>(str, parser, x3::space);
        ensure(cmd);

        return std::move(*cmd);
    }

    size_t calc_hash(const std::string& s)
    {
        size_t hash = 0;
        for (const char c : s)
        {
            hash += c;
            hash *= 17;
            hash %= 256;
        }

        return hash;
    }

    void run_command(box& box, const add_lens& lens)
    {
        if (auto iter = std::ranges::find(box, lens.label, &::lens::label); iter != box.end())
        {
            *iter = lens;
        }
        else
        {
            box.push_back(lens);
        }
    }

    void run_command(box& box, const remove_lens& lens)
    {
        if (auto iter = std::ranges::find(box, lens.label, &::lens::label); iter != box.end())
        {
            box.erase(iter);
        }
    }

    void run_command(state& state, const command& command)
    {
        boost::apply_visitor([&state](const auto& cmd) {
            const auto hash = calc_hash(cmd.label);
            assert(hash < 256);
            box& box = state.box_map[static_cast<box_id>(hash)];
            run_command(box, cmd);
        }, command);
    }

}

namespace aoc
{
namespace year_2023
{

size_t solve15_a(std::istream& input)
{
    auto seqs = std::ranges::views::istream<aoc::getliner<','>>(input);
    return std::ranges::fold_left(seqs | std::ranges::views::transform(&calc_hash), size_t(0), std::plus{});
}

size_t solve15_b(std::istream& input)
{
    auto commands = std::ranges::views::istream<aoc::getliner<','>>(input) | std::ranges::views::transform(&parse_command);

    state state;
    for (const auto& cmd : commands)
    {
        run_command(state, cmd);
    }

    size_t total_power = 0;
    for (const auto& [box_id, box] : state.box_map)
    {
        for (const auto& [slot, lens] : box | std::ranges::views::enumerate)
        {
            const size_t lens_power = (box_id + 1) * (slot + 1) * lens.focal_length;
            total_power += lens_power;
        }
    }

    return total_power;
}

}  // namespace year_2023
}  // namespace aoc
