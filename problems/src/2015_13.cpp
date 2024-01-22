#include "aoc/problems/2015_13.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <execution>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <tuple>

namespace
{
using name       = std::string;
using family_map = std::map<std::tuple<name, name>, int, std::less<>>;

family_map parse_family_map(std::istream& input)
{
    namespace x3 = boost::spirit::x3;

    const auto happiness_action = [](const auto& ctx)
    {
        const auto [ sign, value ] = aoc::x3_attrs_tuple(ctx);

        x3::_val(ctx) = sign * value;
    };

    const x3::symbols<int> happiness_sign{ { "gain", 1 }, { "lose", -1 } };

    const auto name      = x3::lexeme[ +x3::alpha ];
    const auto happiness = x3::rule<struct _hp, int>{} =
        (happiness_sign > x3::int_ > "happiness" > "units")[ happiness_action ];

    family_map map;

    const auto member_action = [ &map ](const auto& ctx)
    {
        auto [ m1, happiness, m2 ] = aoc::x3_attrs_tuple(ctx);
        map.emplace(std::make_tuple(std::move(m1), std::move(m2)), happiness);
    };

    const auto member = x3::rule<struct _mem, ::name>{} =
        (name > "would" > happiness > "by" > "sitting" > "next" > "to" > name > ".")[ member_action ];

    bool parsed = aoc::x3_parse(input, *member, x3::space);
    ensure(parsed);

    return map;
}

int calc_happiness(std::vector<name>& names, const family_map& map)
{
    const auto sz = names.size();

    const auto get_happiness = [ & ](const auto& guest, const int left_idx, const int right_idx)
    {
        const auto& left  = left_idx >= 0 ? names[ left_idx ] : names.back();
        const auto& right = right_idx < sz ? names[ right_idx ] : names.front();

        auto i1 = map.find(std::make_tuple(std::ref(guest), std::ref(left)));
        auto i2 = map.find(std::make_tuple(std::ref(guest), std::ref(right)));

        auto left_happiness  = i1 != map.cend() ? i1->second : 0;
        auto right_happiness = i2 != map.cend() ? i2->second : 0;

        return left_happiness + right_happiness;
    };

    int total_happiness = 0;
    for (int i = 0; i < sz; ++i) { total_happiness += get_happiness(names[ i ], i - 1, i + 1); }

    return total_happiness;
}

int calc_best_happiness(std::vector<name>& names, const family_map& map)
{
    std::sort(names.begin() + 1, names.end());

    int max_happiness = calc_happiness(names, map);
    while (std::next_permutation(names.begin() + 1, names.end()))
    {
        max_happiness = std::max(max_happiness, calc_happiness(names, map));
    }

    return max_happiness;
}

template <bool include_yourself>
size_t solve(std::istream& input)
{
    const auto family_map = parse_family_map(input);
    auto names            = family_map | std::ranges::views::keys |
                 std::ranges::views::transform([](const auto& pair) { return std::get<0>(pair); }) |
                 std::ranges::to<std::vector<name>>();

    if constexpr (include_yourself) { names.push_back("__me__"); }

    std::ranges::sort(names);
    names.erase(std::unique(names.begin(), names.end()), names.end());

    std::vector<std::vector<name>> names_vec;
    for (size_t i = 0; i < names.size() - 1; ++i)
    {
        names_vec.push_back(names);
        std::ranges::rotate(names, names.begin() + 1);
    }
    names_vec.push_back(std::move(names));

#ifdef NDEBUG
    const auto& executor = std::execution::par_unseq;
#else
    const auto& executor = std::execution::seq;
#endif

    size_t best_distance = std::transform_reduce(executor,
                                                 names_vec.begin(),
                                                 names_vec.end(),
                                                 std::numeric_limits<int>::min(),
                                                 std::ranges::max,
                                                 std::bind_back(calc_best_happiness, std::ref(family_map)));
    return best_distance;
}

}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve13_a(std::istream& input)
{
    return solve<false>(input);
}

size_t solve13_b(std::istream& input)
{
    return solve<true>(input);
}

}  // namespace year_2015
}  // namespace aoc
