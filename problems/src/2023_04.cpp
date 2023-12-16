#include "aoc/problems/2023_04.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <functional>
#include <ranges>
#include <set>
#include <unordered_set>
#include <vector>

#include "boost/fusion/adapted/struct.hpp"

namespace
{
using card_id = size_t;

struct card
{
    card_id id;
    std::set<size_t> winning_nums;
    std::set<size_t> nums;
};
}  // namespace

BOOST_FUSION_ADAPT_STRUCT(card, id, winning_nums, nums)

namespace
{
size_t calc_matches(const card& card)
{
    auto winning_nums_iter = card.winning_nums.begin();
    auto nums_iter         = card.nums.begin();

    size_t matches = 0;

    while (winning_nums_iter != card.winning_nums.cend() && nums_iter != card.nums.cend())
    {
        auto winning_num = *winning_nums_iter;
        auto num         = *nums_iter;

        if (winning_num < num) { ++winning_nums_iter; }
        else if (winning_num > num) { ++nums_iter; }
        else
        {
            ++matches;
            ++winning_nums_iter;
            ++nums_iter;
        }
    }

    return matches;
}

size_t calc_score_simple(const card& card, const std::vector<::card>& /*cards*/)
{
    const auto matches = calc_matches(card);
    return matches == 0 ? 0 : (1 << (matches - 1));
}

size_t calc_score_recursive(const card& card, const std::vector<::card>& cards, std::unordered_map<card_id, size_t>& cards_cache)
{
    auto cache_iter = cards_cache.find(card.id);
    if (cache_iter != cards_cache.cend()) { return cache_iter->second; }

    size_t score = 1;

    const auto matches = calc_matches(card);
    if (matches != 0)
    {
        for (size_t i = card.id + 1; i < card.id + 1 + matches; ++i)
        {
            score += calc_score_recursive(cards[ i - 1 ], cards, cards_cache);
        }
    }

    cards_cache.emplace(card.id, score);

    return score;
}

auto make_card_parser()
{
    namespace x3 = boost::spirit::x3;

    return x3::lit("Card") > aoc::x3_size_t_ > x3::lit(':') > +aoc::x3_size_t_ > x3::lit('|') > +aoc::x3_size_t_;
}

template <typename F>
size_t solve(std::istream& input, const F calc_score)
{
    const auto cards        = aoc::parse_lines<card>(input, make_card_parser()) | std::ranges::to<std::vector<card>>();
    const auto points       = cards | std::ranges::views::transform(std::bind_back(calc_score, std::ref(cards)));
    const auto total_points = std::ranges::fold_left(points, size_t(0), std::plus{});

    return total_points;
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve04_a(std::istream& input)
{
    return solve(input, &calc_score_simple);
}

size_t solve04_b(std::istream& input)
{
    std::unordered_map<card_id, size_t> score_cache;
    return solve(input, std::bind_back(&calc_score_recursive, std::ref(score_cache)));
}

}  // namespace year_2023
}  // namespace aoc
