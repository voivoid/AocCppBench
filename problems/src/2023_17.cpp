#include "aoc/problems/2023_17.h"

#include "coords.h"
#include "grid.h"

#include <algorithm>
#include <istream>
#include <mdspan>
#include <queue>
#include <ranges>
#include <vector>

#include <boost/container/static_vector.hpp>

namespace
{
struct state
{
    size_t dist;
    aoc::upoint pos;
    aoc::direction dir;

    bool operator>(const state& state) const
    {
        return dist > state.dist;
    }
};

size_t dir_idx(const aoc::direction dir)
{
    return static_cast<size_t>(dir);
}

struct adjacent_pos
{
    size_t x;
    size_t y;
    aoc::direction dir;
    size_t dist;
};

template <size_t direct_move_min, size_t direct_move_max>
auto get_adjacent_positions(const aoc::grid& grid,
                            const aoc::upoint pos,
                            const aoc::direction dir,
                            const size_t max_x,
                            const size_t max_y)
{
    aoc::rect bounding_rect{ { 0, 0 }, { max_x, max_y } };
    boost::container::static_vector<adjacent_pos, 2 * (direct_move_max - direct_move_min + 1)> adjacents;

    const auto left  = aoc::rotate_left(dir);
    const auto right = aoc::rotate_right(dir);

    size_t dist = 0;
    for (size_t i = 1; i <= direct_move_max; ++i)
    {
        auto next_pos = aoc::get_next_pos(pos, dir, bounding_rect, i);
        if (!next_pos) break;
        dist += grid(next_pos->x, next_pos->y) - '0';

        if (i < direct_move_min) continue;

        adjacents.emplace_back(next_pos->x, next_pos->y, left, dist);
        adjacents.emplace_back(next_pos->x, next_pos->y, right, dist);
    }

    return adjacents;
}

template <size_t direct_move_min, size_t direct_move_max>
size_t solve(std::istream& input)
{
    const auto grid = aoc::grid::read_whole_input(input);

    const auto width         = grid.get_width();
    const auto height        = grid.get_height();
    const aoc::upoint finish = { width - 1, height - 1 };

    const size_t directions = 4;

    size_t inf = size_t(-1);
    std::vector<size_t> distances(width * height * directions, inf);
    std::mdspan dist_span(distances.data(), height, width, directions);
    dist_span[ std::array{ size_t(0), size_t(0), dir_idx(aoc::direction::east) } ]  = 0;
    dist_span[ std::array{ size_t(0), size_t(0), dir_idx(aoc::direction::south) } ] = 0;

    std::priority_queue<state, std::vector<state>, std::greater<state>> pq;
    pq.emplace(0, aoc::upoint{ 0, 0 }, aoc::direction::east);
    pq.emplace(0, aoc::upoint{ 0, 0 }, aoc::direction::south);

    while (!pq.empty())
    {
        const auto state = pq.top();

        if (state.pos == finish) { break; }

        pq.pop();

        for (const auto [ adj_x, adj_y, adj_dir, adj_dist ] : get_adjacent_positions<direct_move_min, direct_move_max>(
                 grid, state.pos, state.dir, width - 1, height - 1))
        {
            auto& best_dist_so_far  = dist_span[ std::array{ adj_y, adj_x, dir_idx(adj_dir) } ];
            const auto current_dist = state.dist + adj_dist;

            if (current_dist < best_dist_so_far)
            {
                best_dist_so_far = current_dist;
                pq.emplace(current_dist, aoc::upoint{ adj_x, adj_y }, adj_dir);
            }
        }
    }

    return pq.top().dist;
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve17_a(std::istream& input)
{
    return solve<1, 3>(input);
}

size_t solve17_b(std::istream& input)
{
    return solve<4, 10>(input);
}

}  // namespace year_2023
}  // namespace aoc
