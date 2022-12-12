#include "aoc/problems/2022_09.h"

#include <iostream>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

#include <boost/functional/hash.hpp>

namespace
{

using dir = char;

struct pos
{
    int x = 0;
    int y = 0;
};

struct state
{
    std::vector<pos> body;
};

void move(pos& pos, const dir dir)
{
    switch (dir)
    {
        case 'L': --pos.x; break;
        case 'R': ++pos.x; break;
        case 'U': --pos.y; break;
        case 'D': ++pos.y; break;
        default: throw std::runtime_error("unknown dir");
    }
}

bool are_touching(const pos& p1, const pos& p2)
{
    return (p1.x >= (p2.x - 1)) && (p1.x <= (p2.x + 1)) && (p1.y >= (p2.y - 1)) && (p1.y <= (p2.y + 1));
}

void move_tail_towards_head(const pos& head, pos& tail)
{
    const auto x_diff = tail.x - head.x;
    const auto y_diff = tail.y - head.y;

    if (std::abs(x_diff) == 2)
    {
        tail.x = (tail.x + head.x) / 2;
    }
    else
    {
        tail.x = head.x;
    }

    if (std::abs(y_diff) == 2)
    {
        tail.y = (tail.y + head.y) / 2;
    }
    else
    {
        tail.y = head.y;
    }
}

size_t solve(std::istream& input, const size_t body_length)
{
    assert(body_length >= 2);

    std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> unique_visited_pos;
    state state{};
    state.body.resize(body_length);

    char dir{};
    size_t steps = 0;
    while (input >> dir >> steps)
    {
        while (steps)
        {
            move(state.body.front(), dir);

            for (auto knot_pair : state.body | std::views::slide(2))
            {
                pos& head = knot_pair[ 0 ];
                pos& tail = knot_pair[ 1 ];

                if (!are_touching(head, tail))
                {
                    move_tail_towards_head(head, tail);
                }
            }

            const auto& tail = state.body.back();
            unique_visited_pos.emplace(tail.x, tail.y);

            --steps;
        }
    }

    return unique_visited_pos.size();
}

}  // namespace

namespace aoc
{
namespace year_2022
{

size_t solve09_a(std::istream& input)
{
    return solve(input, 2);
}

size_t solve09_b(std::istream& input)
{
    return solve(input, 10);
}

}  // namespace year_2022
}  // namespace aoc