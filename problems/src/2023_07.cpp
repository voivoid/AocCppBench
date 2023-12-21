#include "aoc/problems/2023_07.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <ranges>
#include <string>
#include <unordered_map>

#include <boost/container/static_vector.hpp>
#include <boost/fusion/adapted/struct.hpp>

namespace
{
using label = char;
using hand  = std::basic_string<label>;

enum class hand_type
{
    high_card = 0,
    one_pair,
    two_pair,
    three_of_a_kind,
    full_house,
    four_of_a_kind,
    five_of_a_kind,
};

struct parsed_input
{
    hand hand;
    size_t bid;
};

}  // namespace
BOOST_FUSION_ADAPT_STRUCT(parsed_input, hand, bid)

namespace
{

hand_type get_hand_type(const hand& hand)
{
    assert(hand.size() == 5);

    std::unordered_map<label, size_t> cards_map;
    for (const label l : hand) { ++cards_map[ l ]; }

    const auto jokers_iter = cards_map.find('j');
    const auto jokers_num  = jokers_iter == cards_map.end() ? 0 : jokers_iter->second;
    if (jokers_num) cards_map.erase(jokers_iter);

    auto card_counters =
        cards_map | std::ranges::views::values | std::ranges::to<boost::container::static_vector<size_t, 5>>();
    std::ranges::sort(card_counters, std::greater{});

    if (jokers_num)
    {
        if (!card_counters.empty()) { card_counters[ 0 ] += jokers_num; }
        else
        {
            // all 5 cards are jokers
            card_counters.push_back(5);
        }
    }

    switch (card_counters.size())
    {
        case 4: return hand_type::one_pair;
        case 3:
        {
            if (card_counters[ 0 ] == 3)
            {
                assert(card_counters[ 1 ] == 1);
                assert(card_counters[ 2 ] == 1);
                return hand_type::three_of_a_kind;
            }
            else
            {
                assert(card_counters[ 0 ] == 2);
                assert(card_counters[ 1 ] == 2);
                assert(card_counters[ 2 ] == 1);
                return hand_type::two_pair;
            }
        }
        case 2:
        {
            if (card_counters[ 0 ] == 4)
            {
                assert(card_counters[ 1 ] == 1);
                return hand_type::four_of_a_kind;
            }
            else
            {
                assert(card_counters[ 0 ] == 3);
                assert(card_counters[ 1 ] == 2);
                return hand_type::full_house;
            }
        }
        case 1:
        {
            assert(card_counters[ 0 ] == 5);
            return hand_type::five_of_a_kind;
        }
    }

    return hand_type::high_card;
}

struct card_lt_comparer
{
    bool operator()(const label c1, const label c2) const
    {
        return get_card_value(c1) < get_card_value(c2);
    }

    static size_t get_card_value(const label c)
    {
        switch (c)
        {
            case 'A': return 14;
            case 'K': return 13;
            case 'Q': return 12;
            case 'J': return 11;
            case 'T': return 10;
            case 'j': return 1;
        }

        assert(std::isdigit(c));
        return c - '0';
    }
};

struct hand_lt_comparer
{
    bool operator()(const hand& h1, const hand& h2) const
    {
        assert(h1.size() == h2.size());

        const auto t1 = get_hand_type(h1);
        const auto t2 = get_hand_type(h2);
        if (t1 != t2) return t1 < t2;

        return std::lexicographical_compare(h1.cbegin(), h1.cend(), h2.cbegin(), h2.cend(), card_lt_comparer{});
    }
};
}  // namespace



namespace
{

auto make_input_parser(const bool use_jokers = false)
{
    namespace x3 = boost::spirit::x3;

    const auto joker_replace = [ use_jokers ](const auto& ctx)
    {
        const auto label = x3::_attr(ctx);
        x3::_val(ctx)    = ((label == 'J' && use_jokers) ? 'j' : label);
    };

    const auto card_label_parser = x3::rule<struct _label, char>{} = x3::char_[ joker_replace ];
    const auto hand_parser                                         = x3::repeat(5)[ card_label_parser ];
    const auto joker_hand_parser                                   = x3::repeat(5)[ x3::char_ ];
    const auto bid_parser                                          = aoc::x3_size_t_;
    const auto parser                                              = hand_parser > bid_parser;

    return parser;
}

size_t solve(std::istream& input, const bool use_jokers = false)
{
    auto inputs = aoc::parse_lines<parsed_input>(input, make_input_parser(use_jokers)) |
                  std::ranges::to<std::vector<parsed_input>>();
    std::ranges::sort(inputs, hand_lt_comparer{}, &parsed_input::hand);

    const auto ranked_cards = std::ranges::views::zip(inputs, std::ranges::views::iota(1));

    const auto winnings = ranked_cards | std::ranges::views::transform(
                                             [](const auto& v)
                                             {
                                                 const auto [ card, rank ] = v;
                                                 return card.bid * rank;
                                             });

    const auto sum = std::ranges::fold_left(winnings, size_t(0), std::plus{});
    return sum;
}

size_t solve_with_jokers(std::istream& input)
{
    return solve(input, true);
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve07_a(std::istream& input)
{
    return solve(input);
}

size_t solve07_b(std::istream& input)
{
    return solve_with_jokers(input);
}

}  // namespace year_2023
}  // namespace aoc