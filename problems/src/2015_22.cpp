#include "aoc/problems/2015_22.h"

#include "ensure.h"
#include "parse.h"

#include <boost/container/static_vector.hpp>
#include <boost/fusion/adapted/struct.hpp>

#include <algorithm>
#include <cassert>
#include <queue>
#include <vector>

namespace
{
struct player;
struct boss;

struct spell
{
    constexpr spell(int cost, int duration) : mana_cost(cost), duration(duration) {}

    virtual void turn_effect(player&, boss&) const {};
    virtual void start_effect(player&, boss&) const {};
    virtual void end_effect(player&, boss&) const {};

    int mana_cost;
    int duration;
};

struct magic_missle_spell : spell
{
    constexpr magic_missle_spell() : spell(53, 0) {}
    virtual void start_effect(player&, boss& boss) const;
};

static constexpr magic_missle_spell magic_missle;

struct drain_spell : spell
{
    constexpr drain_spell() : spell(73, 0) {}
    virtual void start_effect(player& player, boss& boss) const;
};

static constexpr drain_spell drain;

struct shield_spell : spell
{
    constexpr shield_spell() : spell(113, 6) {}
    virtual void start_effect(player& player, boss&) const;
    virtual void end_effect(player& player, boss&) const;
};

static constexpr shield_spell shield;

struct poison_spell : spell
{
    constexpr poison_spell() : spell(173, 6) {}
    virtual void turn_effect(player&, boss& boss) const;
};

static constexpr poison_spell poison;

struct recharge_spell : spell
{
    constexpr recharge_spell() : spell(229, 5) {}
    virtual void turn_effect(player& player, boss&) const;
};

static constexpr recharge_spell recharge;

struct active_spell
{
    constexpr active_spell(const spell& spell) : instance(&spell), duration_left(spell.duration) {}

    const spell* instance;
    int duration_left;
};

using active_spells = boost::container::static_vector<active_spell, 3>;

struct boss
{
    int hp;
    int damage;
};

struct player
{
    int hp;
    int mana;
    int armor;

    active_spells active_spells;
};

struct state
{
    player player;
    boss boss;
    bool players_turn;
    int mana_spent;
};

void magic_missle_spell::start_effect(player&, boss& boss) const
{
    boss.hp -= 4;
};

void drain_spell::start_effect(player& player, boss& boss) const
{
    player.hp += 2;
    boss.hp -= 2;
};

void shield_spell::start_effect(player& player, boss&) const
{
    player.armor += 7;
};
void shield_spell::end_effect(player& player, boss&) const
{
    player.armor -= 7;
};

void poison_spell::turn_effect(player&, boss& boss) const
{
    boss.hp -= 3;
};

void recharge_spell::turn_effect(player& player, boss&) const
{
    player.mana += 101;
};

constexpr const spell*(all_spells)[]{ &magic_missle, &drain, &shield, &poison, &recharge };

}  // namespace

BOOST_FUSION_ADAPT_STRUCT(boss, hp, damage)

namespace
{
boss parse_boss(std::istream& input)
{
    namespace x3 = boost::spirit::x3;

    const auto make_param_parser = [](const auto name) { return name > ':' > x3::int_; };

    const auto parser = make_param_parser(x3::lit("Hit Points")) > make_param_parser(x3::lit("Damage"));

    const auto boss = aoc::x3_parse_attr<::boss>(input, parser, x3::space);
    ensure(boss);

    return *boss;
}

void handle_active_spells(state& state)
{
    auto& active_spells = state.player.active_spells;
    if (!active_spells.empty())
    {
        for (auto& spell : active_spells)
        {
            spell.instance->turn_effect(state.player, state.boss);

            --spell.duration_left;
            if (spell.duration_left == 0) spell.instance->end_effect(state.player, state.boss);
        }

        auto to_erase = std::ranges::remove(active_spells, 0, &active_spell::duration_left);
        active_spells.erase(to_erase.begin(), to_erase.end());
    }
}

auto get_available_spells(const player& player)
{
    return all_spells | std::ranges::views::filter(
                            [ & ](const spell* spell)
                            {
                                if (spell->mana_cost > player.mana) return false;
                                if (spell->duration == 0) return true;

                                for (const auto& active_spell : player.active_spells)
                                {
                                    if (active_spell.instance == spell) return false;
                                }
                                return true;
                            });
}

void cast_spell(state& state, const spell& spell)
{
    spell.start_effect(state.player, state.boss);
    state.mana_spent += spell.mana_cost;
    state.player.mana -= spell.mana_cost;

    if (spell.duration != 0) { state.player.active_spells.push_back(spell); }
}

void process_boss_attack(state& state)
{
    const auto boss_damage = std::max(1, state.boss.damage - state.player.armor);
    state.player.hp -= boss_damage;
}

template <bool hard_mode>
void find_least_mana(state state, int& least_mana_so_far)
{
    if constexpr (hard_mode)
    {
        if (state.players_turn) --state.player.hp;
    }

    if (state.player.hp <= 0) return;
    if (state.mana_spent >= least_mana_so_far) return;

    state.players_turn = !state.players_turn;

    handle_active_spells(state);

    if (state.boss.hp <= 0)
    {
        least_mana_so_far = std::min(least_mana_so_far, state.mana_spent);
        return;
    }

    if (state.players_turn)
    {
        auto available_spells = get_available_spells(state.player);

        for (const spell* spell : available_spells)
        {
            auto next_state = state;
            cast_spell(next_state, *spell);

            find_least_mana<hard_mode>(std::move(next_state), least_mana_so_far);
        }
    }
    else
    {
        process_boss_attack(state);
        find_least_mana<hard_mode>(std::move(state), least_mana_so_far);
    }
}

template <bool hard_mode>
size_t solve(std::istream& input)
{
    const auto initial_boss   = parse_boss(input);
    const auto initial_player = player{ .hp = 50, .mana = 500, .armor = 0 };

    const state initial_state{ initial_player, initial_boss, false, 0 };

    int least_mana_so_far = std::numeric_limits<int>::max();
    find_least_mana<hard_mode>(initial_state, least_mana_so_far);

    return least_mana_so_far;
}
}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve22_a(std::istream& input)
{
    return solve<false>(input);
}

size_t solve22_b(std::istream& input)
{
    return solve<true>(input);
}

}  // namespace year_2015
}  // namespace aoc
