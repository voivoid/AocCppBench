#include "aoc/problems/2015_17.h"

#include <algorithm>
#include <cassert>
#include <istream>
#include <ranges>
#include <unordered_map>
#include <vector>

#include <boost/functional/hash.hpp>



namespace
{

struct result
{
    size_t combinations;
    size_t containers;

    static constexpr size_t irrelevant = size_t(-1);
};

using containers = std::vector<size_t>;
using cache      = std::unordered_map<std::pair<size_t, size_t>, size_t, boost::hash<std::pair<size_t, size_t>>>;

size_t find_all_combinations(const containers& containers, const size_t idx, const size_t total_vol, cache& cache)
{
    if (total_vol == 0) return 1;
    if (idx == containers.size()) return 0;
    if (containers[ idx ] > total_vol) return 0;

    const auto cache_key = std::make_pair(idx, total_vol);
    auto cache_iter      = cache.find(cache_key);
    if (cache_iter != cache.cend()) return cache_iter->second;

    const auto result = find_all_combinations(containers, idx + 1, total_vol, cache) +
                        find_all_combinations(containers, idx + 1, total_vol - containers[ idx ], cache);

    cache.emplace(cache_key, result);

    return result;
}

size_t counter = 0;

result find_all_min_containers_combinations(const containers& containers,
                                            const size_t containers_used,
                                            size_t& min_containers_so_far,
                                            const size_t idx,
                                            const size_t total_vol)
{
    if (containers_used > min_containers_so_far) return { 0, result::irrelevant };
    if (total_vol == 0)
    {
        min_containers_so_far = std::min(containers_used, min_containers_so_far);
        return { 1, containers_used };
    }

    if (idx == containers.size()) return { 0, result::irrelevant };
    if (containers[ idx ] > total_vol) return { 0, result::irrelevant };

    auto [ r1, used1 ] =
        find_all_min_containers_combinations(containers, containers_used, min_containers_so_far, idx + 1, total_vol);
    auto [ r2, used2 ] = find_all_min_containers_combinations(
        containers, containers_used + 1, min_containers_so_far, idx + 1, total_vol - containers[ idx ]);

    size_t result_combinations = 0;
    if (used1 == min_containers_so_far) result_combinations += r1;
    if (used2 == min_containers_so_far) result_combinations += r2;

    return { result_combinations, min_containers_so_far };
}

}  // namespace

namespace aoc
{
namespace year_2015
{

namespace details
{
size_t solve17_a(std::istream& input, const size_t total_vol)
{
    auto containers = std::ranges::views::istream<size_t>(input) | std::ranges::to<::containers>();
    std::ranges::sort(containers);

    cache cache;
    return find_all_combinations(containers, 0, total_vol, cache);
}

size_t solve17_b(std::istream& input, size_t total_vol)
{
    auto containers = std::ranges::views::istream<size_t>(input) | std::ranges::to<::containers>();
    std::ranges::sort(containers);

    size_t min = containers.size();
    return find_all_min_containers_combinations(containers, 0, min, 0, total_vol).combinations;
}
}  // namespace details

size_t solve17_a(std::istream& input)
{
    return details::solve17_a(input, 150);
}

size_t solve17_b(std::istream& input)
{
    return details::solve17_b(input, 150);
}

}  // namespace year_2015
}  // namespace aoc
