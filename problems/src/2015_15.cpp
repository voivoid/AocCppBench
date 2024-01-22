#include "aoc/problems/2015_15.h"

#include "ensure.h"
#include "fusion.h"
#include "parse.h"

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/tuple.hpp>

#include <execution>
#include <functional>
#include <istream>
#include <numeric>
#include <ranges>
#include <valarray>
#include <vector>

#include <boost/container/small_vector.hpp>

namespace
{
constexpr int teaspoons_per_recipe = 100;

using ingredient  = std::valarray<int>;
using ingredients = boost::container::small_vector<ingredient, 4>;

enum property_idx
{
    capacity   = 0,
    durability = 1,
    flavor     = 2,
    texture    = 3,
    calories   = 4,
    last_property
};

auto make_parser()
{
    namespace x3 = boost::spirit::x3;

    const auto make_property_parser = [](const auto name, property_idx idx)
    { return x3::omit[ name ] > x3::int_ > x3::attr(idx); };

    const auto name       = x3::lexeme[ +x3::alpha ];
    const auto capacity   = make_property_parser(x3::lit("capacity"), property_idx::capacity);
    const auto durability = make_property_parser(x3::lit("durability"), property_idx::durability);
    const auto flavor     = make_property_parser(x3::lit("flavor"), property_idx::flavor);
    const auto texture    = make_property_parser(x3::lit("texture"), property_idx::texture);
    const auto calories   = make_property_parser(x3::lit("calories"), property_idx::calories);
    const auto prop       = x3::rule<struct _prop, std::tuple<int, property_idx>>{} =
        capacity | durability | flavor | texture | calories;

    const auto component_action = [](const auto& ctx)
    {
        for (const auto& prop : boost::spirit::x3::_attr(ctx))
        {
            auto& val = x3::_val(ctx);
            if (val.size() == 0) { val.resize(property_idx::last_property); }
            const auto [ value, prop_idx ] = aoc::fusion_to_std_tuple(prop);
            val[ prop_idx ]                = value;
        }
    };

    const auto component = x3::rule<struct _component, ::ingredient>{} =
        (x3::omit[ name ] > ':' > x3::repeat(5)[ prop >> -x3::lit(',') ])[ component_action ];

    return component;
}

template <int calories_limit>
size_t calc_score(std::vector<int>& teaspoons, ingredients ingredients)
{
    assert(teaspoons.size() == ingredients.size());

    for (size_t i = 0; i < teaspoons.size(); ++i) { ingredients[ i ] *= teaspoons[ i ]; }

    auto initial = ingredient{};
    initial.resize(property_idx::last_property);

    auto properties_sum = std::ranges::fold_left(ingredients, initial, std::plus{});
    if constexpr (calories_limit != 0)
    {
        if (properties_sum[ property_idx::calories ] != calories_limit) return 0;
    }

    if (std::ranges::any_of(properties_sum, [](const auto p) { return p <= 0; })) { return 0; }

    const auto product =
        std::accumulate(std::begin(properties_sum), std::end(properties_sum) - 1, 1, std::multiplies{});
    return product;
}

template <int calories_limit>
size_t find_max_score_for_all_teaspoon_combinations(std::vector<int>& teaspoons,
                                                    const int teaspoons_left,
                                                    const ingredients& ingredients)
{
    if (teaspoons.size() == ingredients.size() - 1)
    {
        teaspoons.push_back(teaspoons_left);
        const auto result = calc_score<calories_limit>(teaspoons, ingredients);
        teaspoons.pop_back();
        return result;
    }

    assert(teaspoons_left > ingredients.size());
    const auto available_teaspoons = teaspoons_left - ingredients.size();

    size_t result = 0;
    for (int i = 0; i < available_teaspoons; ++i)
    {
        teaspoons.push_back(i);
        result = std::max(
            result,
            find_max_score_for_all_teaspoon_combinations<calories_limit>(teaspoons, teaspoons_left - i, ingredients));
        teaspoons.pop_back();
    }

    return result;
}

template <int calories_limit>
size_t find_max_score(const int first_ingredient_teaspoons, const ingredients& ingredients)
{
    std::vector<int> teaspoons;
    teaspoons.reserve(ingredients.size());
    teaspoons.push_back(first_ingredient_teaspoons);

    return find_max_score_for_all_teaspoon_combinations<calories_limit>(
        teaspoons, teaspoons_per_recipe - first_ingredient_teaspoons, ingredients);
}

template <int calories_limit>
size_t solve(std::istream& input)
{
    const auto ingredients = aoc::parse_lines<ingredient>(input, make_parser()) | std::ranges::to<::ingredients>();
    const auto ns          = std::views::iota(1, teaspoons_per_recipe - static_cast<int>(ingredients.size()) - 1 - 1);

#ifdef NDEBUG
    const auto& executor = std::execution::par_unseq;
#else
    const auto& executor = std::execution::seq;
#endif

    const auto max_score = std::transform_reduce(executor,
                                                 ns.begin(),
                                                 ns.end(),
                                                 size_t(0),
                                                 std::ranges::max,
                                                 std::bind_back(find_max_score<calories_limit>, std::ref(ingredients)));

    return max_score;
}

}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve15_a(std::istream& input)
{
    return solve<0>(input);
}

size_t solve15_b(std::istream& input)
{
    return solve<500>(input);
}

}  // namespace year_2015
}  // namespace aoc
