#include "aoc/problems/2023_12.h"

#include "ensure.h"
#include "parse.h"

#include <execution>
#include <istream>
#include <numeric>
#include <optional>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_map>

#include <boost/functional/hash.hpp>
#include <boost/fusion/adapted/struct.hpp>

namespace
{
struct row
{
    std::string springs;
    std::vector<size_t> damage_groups;
};
}  // namespace


BOOST_FUSION_ADAPT_STRUCT(row, springs, damage_groups)

namespace
{
auto make_row_parser()
{
    namespace x3              = boost::spirit::x3;
    const auto springs_parser = +x3::char_("?#.");
    const auto damages_parser = aoc::x3_size_t_ % ',';

    const auto parser = x3::rule<struct _row, row>{} = springs_parser > damages_parser;
    return parser;
}

struct traversal_state
{
    char current_char;
    size_t current_spring_idx;
    std::optional<size_t> match_in_progress;
    size_t current_damage_group_idx;

    struct hash
    {
        std::size_t operator()(const traversal_state& s) const noexcept
        {
            std::size_t seed = 0;
            boost::hash_combine(seed, s.current_spring_idx);
            boost::hash_combine(seed, s.current_char);
            boost::hash_combine(seed, s.match_in_progress);
            boost::hash_combine(seed, s.current_damage_group_idx);
            return seed;
        }
    };

    bool operator==(const traversal_state&) const = default;
};

using visited_map = std::unordered_map<traversal_state, size_t, traversal_state::hash>;

size_t calc_arrangements(const row& row, traversal_state state, visited_map& visited)
{
    const size_t total_springs       = row.springs.size();
    const size_t total_damage_groups = row.damage_groups.size();
    const auto initial_state         = state;
    const bool use_cache             = initial_state.current_char == '?';

    if (use_cache)
    {
        if (auto visited_iter = visited.find(state); visited_iter != visited.cend()) { return visited_iter->second; }
    }

    const auto goto_next_char = [ &row, &state ]() { state.current_char = row.springs[ ++state.current_spring_idx ]; };

    while (state.match_in_progress && state.current_spring_idx != total_springs)
    {
        auto& matches_left = *state.match_in_progress;

        if (state.current_char == '#')
        {
            if (matches_left > 0)
            {
                --matches_left;
                goto_next_char();
            }
            else
                return 0;
        }
        else if (state.current_char == '.')
        {
            if (matches_left != 0) return 0;
            else
                state.match_in_progress.reset();

            goto_next_char();
        }
        else if (state.current_char == '?')
        {
            if (matches_left == 0) state.match_in_progress.reset();
            else
                --matches_left;

            goto_next_char();
        }
    }

    if (state.current_spring_idx == total_springs)
    {
        const bool success_match = state.current_damage_group_idx == total_damage_groups &&
                                   (!state.match_in_progress || state.match_in_progress == 0);

        return success_match ? 1 : 0;
    }

    size_t arrangements = 0;
    switch (state.current_char)
    {
        case '#':
        {
            if (state.current_damage_group_idx < total_damage_groups)
            {
                state.match_in_progress = row.damage_groups[ state.current_damage_group_idx++ ] - 1;
                goto_next_char();
            }
            else
                return 0;

            break;
        }
        case '.':
        {
            goto_next_char();
            break;
        }
        case '?':
        {
            state.current_char = '.';
            arrangements += calc_arrangements(row, state, visited);

            state.current_char = '#';
            break;
        }
    }

    arrangements += calc_arrangements(row, state, visited);

    if (use_cache) { visited.emplace(initial_state, arrangements); }

    return arrangements;
}

size_t calc_row_arrangements1(const row& row)
{
    traversal_state initial = { row.springs.front(), 0, {}, 0 };
    visited_map visited;
    return calc_arrangements(row, initial, visited);
}

size_t calc_row_arrangements2(const row& row)
{
    std::string unfolded_springs = std::ranges::views::repeat(row.springs, 5) | std::ranges::views::join_with('?') |
                                   std::ranges::to<std::string>();
    std::vector<size_t> unfolded_damage_groups = std::ranges::views::repeat(row.damage_groups, 5) |
                                                 std::ranges::views::join | std::ranges::to<std::vector<size_t>>();

    return calc_row_arrangements1({ std::move(unfolded_springs), std::move(unfolded_damage_groups) });
}

template <auto calc>
size_t solve(std::istream& input)
{
    const auto rows         = aoc::parse_lines<row>(input, make_row_parser());
    const auto arrangements = rows | std::ranges::views::transform(calc);
    return std::ranges::fold_left(arrangements, size_t(0), std::plus{});
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve12_a(std::istream& input)
{
    return solve<&calc_row_arrangements1>(input);
}

size_t solve12_b(std::istream& input)
{
    return solve<&calc_row_arrangements2>(input);
}

}  // namespace year_2023
}  // namespace aoc
