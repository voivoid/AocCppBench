#include "aoc/problems/2023_10.h"

#include "coords.h"

#include <algorithm>
#include <istream>
#include <map>
#include <mdspan>
#include <ranges>
#include <set>
#include <string>
#include <vector>

#include <boost/container/static_vector.hpp>

namespace
{

using coord = size_t;

boost::container::static_vector<aoc::upoint, 2> get_next_steps(const coord x, const coord y, const auto& maze2d)
{
    const auto width        = maze2d.extent(1);
    const auto height       = maze2d.extent(0);
    const auto current_tile = maze2d[ std::array{ y, x } ];

    struct neighbour
    {
        aoc::upoint next_coord;
        std::string_view valid_current_pipes;
        std::string_view valid_neighbour_pipes;
    };

    const std::initializer_list<neighbour> neighbours = { { { x - 1, y }, "-J7", "-LF" },
                                                          { { x + 1, y }, "-LF", "-J7" },
                                                          { { x, y - 1 }, "|LJ", "|7F" },
                                                          { { x, y + 1 }, "|7F", "|LJ" } };

    boost::container::static_vector<aoc::upoint, 2> next_steps;
    for (const auto& n : neighbours)
    {
        const auto [ nx, ny ] = n.next_coord;
        if (nx >= width || ny >= height) continue;
        if (!n.valid_current_pipes.contains(current_tile) && current_tile != 'S') continue;

        const auto next_pipe = maze2d[ std::array{ ny, nx } ];
        if (!n.valid_neighbour_pipes.contains(next_pipe)) continue;

        next_steps.push_back(n.next_coord);
    }

    return next_steps;
}

aoc::upoint get_next_step(const coord x, const coord y, const auto& maze2d, const aoc::upoint prev_step)
{
    const auto steps = get_next_steps(x, y, maze2d);
    if (steps[ 0 ] == prev_step) return steps[ 1 ];

    assert(steps[ 1 ] == prev_step);
    return steps[ 0 ];
}

char find_start_pipe_tile(const aoc::upoint start_point, const auto& maze2d)
{
    const auto x = start_point.x;
    const auto y = start_point.y;

    const auto neighbours = get_next_steps(x, y, maze2d);
    assert(neighbours.size() == 2);

    const auto p1 = neighbours[ 0 ];
    const auto p2 = neighbours[ 1 ];

    char pipe_tile{};
    if (p1.x == p2.x) pipe_tile = '|';
    else if (p1.y == p2.y)
        pipe_tile = '-';
    else
    {
        const auto left   = p1.x < x || p2.x < x;
        const auto right  = p1.x > x || p2.x > x;
        const auto top    = p1.y < y || p2.y < y;
        const auto bottom = p1.y > y || p2.y > y;

        if (left && top) pipe_tile = 'J';
        else if (right && top)
            pipe_tile = 'L';
        else if (left && bottom)
            pipe_tile = '7';
        else if (right && bottom)
            pipe_tile = 'F';
        else { assert(false); }
    }

    return pipe_tile;
}

std::tuple<std::string, size_t, size_t, aoc::upoint> read_maze(std::istream& input)
{
    std::string maze_str;
    maze_str.reserve(141 * 141);
    std::getline(input, maze_str);
    const size_t width = maze_str.size();
    maze_str.append(std::istream_iterator<char>(input), std::istream_iterator<char>());

    assert(maze_str.size() % width == 0);
    const size_t height = maze_str.size() / width;

    const auto start_pos = maze_str.find('S');
    assert(start_pos != std::string::npos);
    const aoc::upoint start_coord = { start_pos % width, start_pos / width };

    std::mdspan maze2d(maze_str.data(), height, width);

    const char start_pipe_tile                           = find_start_pipe_tile(start_coord, maze2d);
    maze2d[ std::array{ start_coord.y, start_coord.x } ] = start_pipe_tile;

    return { std::move(maze_str), width, height, start_coord };
}

using loop_map = std::map<coord, std::set<coord>>;

bool is_already_visited(const loop_map& visited, const coord x, const coord y)
{
    auto y_iter = visited.find(y);
    if (y_iter == visited.cend()) return false;

    return y_iter->second.contains(x);
}

loop_map get_loop_map(const aoc::upoint start_coord, const auto& maze)
{
    std::map<coord, std::set<coord>> visited;

    const auto next_steps = get_next_steps(start_coord.x, start_coord.y, maze);
    assert(next_steps.size() == 2);

    aoc::upoint current_step = start_coord;
    aoc::upoint prev_step    = next_steps[ 0 ];

    do {
        const auto cx = current_step.x;
        const auto cy = current_step.y;
        visited[ cy ].insert(cx);

        auto next_step = get_next_step(cx, cy, maze, prev_step);
        prev_step      = current_step;
        current_step   = next_step;
    } while (current_step != start_coord);

    return { std::move(visited) };
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve10_a(std::istream& input)
{
    const auto [ maze_str, width, height, start_coord ] = read_maze(input);
    std::mdspan maze(maze_str.data(), height, width);

    const auto loop_map = get_loop_map(start_coord, maze);
    const auto tiles =
        loop_map | std::ranges::views::values | std::ranges::views::transform([](const auto& m) { return m.size(); });
    const auto tiles_total = std::ranges::fold_left(tiles, size_t(0), std::plus{});

    return tiles_total / 2;
}

size_t solve10_b(std::istream& input)
{
    const auto [ maze_str, width, height, start_coord ] = read_maze(input);
    std::mdspan maze2d(maze_str.data(), height, width);

    const auto loop_map = get_loop_map(start_coord, maze2d);

    struct pipe
    {
        size_t x;
        char tile;
    };

    size_t enclosed_sum = 0;

    for (const auto& [ y, row ] : loop_map)
    {
        std::optional<size_t> inside_loop_from;
        size_t skip = 0;
        for (const auto x : row)
        {
            const auto pipe       = maze2d[ std::array{ y, x } ];
            const bool cross_loop = pipe == '|' || pipe == 'L' || pipe == 'J';

            if (cross_loop && !inside_loop_from) { inside_loop_from = x; }
            else if (cross_loop && inside_loop_from)
            {
                const auto diff = x - *inside_loop_from - skip - 1;
                enclosed_sum += diff;

                inside_loop_from.reset();
                skip = 0;
            }
            else if (inside_loop_from) { ++skip; }
        }
    }

    return enclosed_sum;
}

}  // namespace year_2023
}  // namespace aoc
