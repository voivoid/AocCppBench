#include "aoc/problems/2022_12.h"

#include "coord.h"

#include <cassert>
#include <iostream>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

#include <boost/container/static_vector.hpp>

namespace
{
using grid = std::vector<char>;

struct area
{
    grid grid;
    size_t rows;
    size_t cols;

    aoc::pos start;
    aoc::pos finish;
};

struct step
{
    aoc::pos pos;
    size_t n;
};

aoc::pos get_pos_by_index(const size_t idx, const size_t cols)
{
    const auto x = idx % cols;
    const auto y = idx / cols;

    return aoc::pos{ x, y };
}

size_t get_index_by_pos(const aoc::pos& pos, const area& area)
{
    return pos.y * area.cols + pos.x;
}

char get_height(const aoc::pos& pos, const area& area)
{
    const auto h = area.grid[ get_index_by_pos(pos, area) ];

    if (h == 'S')
        return 'a';
    else if (h == 'E')
        return 'z';
    return h;
}

area parse_area(std::istream& input)
{
    std::string line;
    std::getline(input, line);

    std::vector<char> grid(line.cbegin(), line.cend());
    grid.insert(grid.end(), std::istream_iterator<char>{ input }, std::istream_iterator<char>{});

    area area{};
    area.cols = line.size();
    area.rows = grid.size() / area.cols;
    area.grid = std::move(grid);

    const auto grid_size = area.grid.size();
    for (size_t i = 0; i < grid_size; ++i)
    {
        if (area.grid[ i ] == 'S')
        {
            area.start = get_pos_by_index(i, area.cols);
        }
        else if (area.grid[ i ] == 'E')
        {
            area.finish = get_pos_by_index(i, area.cols);
        }
    }

    assert(area.start != area.finish);

    return area;
}

bool can_step_uphill(const aoc::pos& from, const aoc::pos& to, const area& area)
{
    const auto from_height = get_height(from, area);
    const auto to_height   = get_height(to, area);
    return (to_height - from_height) <= 1;
}

bool can_step_downhill(const aoc::pos& from, const aoc::pos& to, const area& area)
{
    const auto from_height = get_height(from, area);
    const auto to_height   = get_height(to, area);
    return from_height - to_height <= 1;
}

template <bool (*can_step)(const aoc::pos&, const aoc::pos&, const area&)>
boost::container::static_vector<aoc::pos, 4> get_neighbours(const aoc::pos& p, const area& area)
{
    boost::container::static_vector<aoc::pos, 4> neighbours;

    const aoc::pos left{ p.x - 1, p.y };
    const aoc::pos up{ p.x, p.y - 1 };
    const aoc::pos right{ p.x + 1, p.y };
    const aoc::pos down{ p.x, p.y + 1 };

    if (p.x != 0 && can_step(p, left, area))
        neighbours.emplace_back(left);
    if (p.y != 0 && can_step(p, up, area))
        neighbours.emplace_back(up);

    if (p.x != area.cols - 1 && can_step(p, right, area))
        neighbours.emplace_back(right);
    if (p.y != area.rows - 1 && can_step(p, down, area))
        neighbours.emplace_back(down);

    return neighbours;
}

bool is_finish(const aoc::pos& pos, const area& area)
{
    return pos == area.finish;
}

bool is_lowland(const aoc::pos& pos, const area& area)
{
    return get_height(pos, area) == 'a';
}

template <bool (*should_stop)(const aoc::pos&, const area&), bool (*can_step)(const aoc::pos&, const aoc::pos&, const area&)>
size_t find_shortest_path(const area& area, const aoc::pos& start)
{
    std::queue<step> queue;

    queue.emplace(start, 0);

    std::unordered_set<aoc::pos> visited;

    while (!queue.empty())
    {
        const auto current_step = queue.front();
        if (should_stop(current_step.pos, area))
        {
            return current_step.n;
        }

        queue.pop();

        if (visited.contains(current_step.pos))
        {
            continue;
        }
        visited.insert(current_step.pos);

        const auto neighbours = get_neighbours<can_step>(current_step.pos, area);
        for (const auto neighbour : neighbours)
        {
            if (!visited.contains(neighbour))
            {
                queue.emplace(neighbour, current_step.n + 1);
            }
        }
    }

    throw std::runtime_error("no solution");
}

}  // namespace

namespace aoc
{
namespace year_2022
{

size_t solve12_a(std::istream& input)
{
    const auto area = parse_area(input);
    return find_shortest_path<is_finish, can_step_uphill>(area, area.start);
}

size_t solve12_b(std::istream& input)
{
    const auto area = parse_area(input);
    return find_shortest_path<is_lowland, can_step_downhill>(area, area.finish);
}

}  // namespace year_2022
}  // namespace aoc