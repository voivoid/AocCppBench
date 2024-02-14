#include "aoc/problems/2015_21.h"

#include "ensure.h"
#include "functors.h"
#include "macros.h"
#include "parse.h"

AOC_MSVC_DISABLE_WARNING(4127)  // conditional expression is constant

#include <boost/fusion/adapted/struct.hpp>

#include <algorithm>
#include <ranges>
#include <span>

namespace
{
struct fighter
{
    int hp;
    int damage;
    int armor;
};

struct item
{
    size_t cost;
    int damage;
    int armor;

    constexpr auto operator<=>(const item& rhs) const
    {
        return cost <=> rhs.cost;
    }

    constexpr item operator+(const item& rhs) const
    {
        return item{ .cost = cost + rhs.cost, .damage = damage + rhs.damage, .armor = armor + rhs.armor };
    }
};

using stats = item;

static constexpr item weapons[] = { { 8, 4, 0 }, { 10, 5, 0 }, { 25, 6, 0 }, { 40, 7, 0 }, { 74, 8, 0 } };
static_assert(std::ranges::is_sorted(weapons, {}, &item::cost));

static constexpr item armors[] = { { 0, 0, 0 }, { 13, 0, 1 }, { 31, 0, 2 }, { 53, 0, 3 }, { 75, 0, 4 }, { 102, 0, 5 } };
static_assert(std::ranges::is_sorted(armors, {}, &item::cost));

static constexpr item rings[] = { { 0, 0, 0 },  { 25, 1, 0 }, { 50, 2, 0 }, { 100, 3, 0 },
                                  { 20, 0, 1 }, { 40, 0, 2 }, { 80, 0, 3 } };

constexpr auto make_ring_pairs()
{
    const size_t rings_num = std::size(rings);
    std::array<item, 1 + (rings_num * (rings_num - 1)) / 2> pairs;

    pairs[ 0 ] = item{ 0, 0, 0 };  // no rings

    size_t c = 1;
    for (size_t i = 0; i < rings_num; ++i)
    {
        for (size_t j = i + 1; j < rings_num; ++j) { pairs[ c++ ] = rings[ i ] + rings[ j ]; }
    }

    std::sort(pairs.begin(), pairs.end());

    return pairs;
}
static constexpr auto ring_pairs = make_ring_pairs();
static_assert(std::ranges::is_sorted(ring_pairs, {}, &item::cost));


int calc_turns_to_win(const int attacker_damage, const int opponent_hp)
{
    const auto [ div, rem ] = std::div(opponent_hp, attacker_damage);
    return div + (rem == 0 ? 0 : 1);
}

bool will_player_win(const fighter& player, const fighter& boss)
{
    const auto player_deals_damage = std::max(1, player.damage - boss.armor);
    const auto boss_deals_damage   = std::max(1, boss.damage - player.armor);

    const auto player_turns_to_win = calc_turns_to_win(player_deals_damage, boss.hp);
    const auto boss_turns_to_win   = calc_turns_to_win(boss_deals_damage, player.hp);

    return player_turns_to_win <= boss_turns_to_win;
}

bool will_player_win(const stats& player_stats, const fighter& boss)
{
    fighter player{ .hp = 100, .damage = player_stats.damage, .armor = player_stats.armor };
    return will_player_win(player, boss);
}

}  // namespace

BOOST_FUSION_ADAPT_STRUCT(fighter, hp, damage, armor)

namespace
{
fighter parse_boss(std::istream& input)
{
    namespace x3 = boost::spirit::x3;

    const auto make_param_parser = [](const auto name) { return name > ':' > x3::int_; };

    const auto parser = make_param_parser(x3::lit("Hit Points")) > make_param_parser(x3::lit("Damage")) >
                        make_param_parser(x3::lit("Armor"));

    const auto boss = aoc::x3_parse_attr<fighter>(input, parser, x3::space);
    ensure(boss);

    return *boss;
}

template <bool player_should_win>
size_t solve(std::istream& input)
{
    const auto boss = parse_boss(input);
    const std::vector<std::span<const item>> item_sets{ weapons, armors, ring_pairs };

    static constexpr auto choose_best_cost =
        std::conditional_t<player_should_win, aoc::functors::min, aoc::functors::max>{};

    size_t best_cost_so_far = player_should_win ? std::numeric_limits<size_t>::max() : 0;

    const auto find_best_cost_recursive =
        [ & ](this auto& recursive_call, const size_t set_idx, const stats current_stats) -> void
    {
        if (player_should_win && choose_best_cost(current_stats.cost, best_cost_so_far) == best_cost_so_far) return;

        if (set_idx == item_sets.size())  // are all items selected?
        {
            if (!(player_should_win ^ will_player_win(current_stats, boss)))
            {
                best_cost_so_far = choose_best_cost(current_stats.cost, best_cost_so_far);
            }
            return;
        }

        for (const auto& item : item_sets[ set_idx ]) { recursive_call(set_idx + 1, current_stats + item); }
    };

    find_best_cost_recursive(0, stats{});
    return best_cost_so_far;
}

}  // namespace

namespace aoc
{
namespace year_2015
{
size_t solve21_a(std::istream& input)
{
    return solve<true>(input);
}

size_t solve21_b(std::istream& input)
{
    return solve<false>(input);
}

}  // namespace year_2015
}  // namespace aoc
