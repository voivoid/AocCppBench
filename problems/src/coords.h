#pragma once

#include <algorithm>
#include <functional>
#include <limits>

#include <boost/container/static_vector.hpp>
#include <boost/functional/hash.hpp>

namespace aoc
{

enum class direction
{
    north    = 0,
    up       = 0,
    east     = 1,
    right    = 1,
    forward  = 1,
    south    = 2,
    down     = 2,
    west     = 3,
    left     = 3,
    backward = 3
};

template <typename Coord>
struct generic_point
{
    Coord x;
    Coord y;
};

template <typename T>
struct generic_rect
{
    generic_rect(aoc::generic_point<T> left_top, aoc::generic_point<T> right_bottom) :
        left(left_top.x), right(right_bottom.x), top(left_top.y), bottom(right_bottom.y)
    {
    }

    T left;
    T right;
    T top;
    T bottom;
};

using upoint = aoc::generic_point<size_t>;
using point  = aoc::generic_point<long long>;
using rect   = aoc::generic_rect<size_t>;

template <typename T>
bool operator<(const aoc::generic_point<T>& l1, const aoc::generic_point<T>& l2)
{
    return l1.x != l2.x ? l1.x < l2.x : l1.y < l2.y;
}

template <typename T>
bool operator==(const aoc::generic_point<T>& l1, const aoc::generic_point<T>& l2)
{
    return l1.x == l2.x && l1.y == l2.y;
}

template <typename T>
bool operator!=(const aoc::generic_point<T>& l1, const aoc::generic_point<T>& l2)
{
    return !(l1 == l2);
}

inline bool is_horizontal_direction(const aoc::direction dir)
{
    return dir == direction::east || dir == direction::west;
}

inline bool is_vertical_direction(const aoc::direction dir)
{
    return dir == direction::north || dir == direction::south;
}

inline aoc::direction get_opposite_direction(const aoc::direction dir)
{
    switch (dir)
    {
        case direction::north: return direction::south;
        case direction::east: return direction::west;
        case direction::west: return direction::east;
        case direction::south: return direction::north;
    }

    std::unreachable();
}

template <typename T>
size_t manhattan_distance(const aoc::generic_point<T>& l1, const aoc::generic_point<T>& l2)
{
    const auto [ xmin, xmax ] = std::minmax(l1.x, l2.x);
    const auto [ ymin, ymax ] = std::minmax(l1.y, l2.y);
    return (xmax - xmin) + (ymax - ymin);
}

inline aoc::direction rotate_left(const aoc::direction dir)
{
    switch (dir)
    {
        case direction::north: return direction::west;
        case direction::west: return direction::south;
        case direction::south: return direction::east;
        case direction::east: return direction::north;
    }

    std::unreachable();
}

inline aoc::direction rotate_right(const aoc::direction dir)
{
    switch (dir)
    {
        case direction::north: return direction::east;
        case direction::east: return direction::south;
        case direction::south: return direction::west;
        case direction::west: return direction::north;
    }

    std::unreachable();
}


template <typename T>
aoc::generic_point<T> get_next_pos(const aoc::generic_point<T>& p, const aoc::direction dir, T steps = 1)
{
    assert(steps > 0);
    switch (dir)
    {
        case direction::north: return { p.x, p.y - steps };
        case direction::east: return { p.x + steps, p.y };
        case direction::south: return { p.x, p.y + steps };
        case direction::west: return { p.x - steps, p.y };
    };

    std::unreachable();
}

template <typename T>
std::optional<aoc::generic_point<T>> get_next_pos(const aoc::generic_point<T>& p,
                                                  const aoc::direction dir,
                                                  const aoc::generic_rect<T>& bounding_rect,
                                                  T steps = 1)
{
    assert(steps > 0);
    switch (dir)
    {
        case direction::north:
            if (p.y > (bounding_rect.top + steps - 1)) return get_next_pos(p, dir, steps);
            return {};
        case direction::east:
            if (p.x < (bounding_rect.right + 1 - steps)) return get_next_pos(p, dir, steps);
            return {};
        case direction::south:
            if (p.y < (bounding_rect.bottom + 1 - steps)) return get_next_pos(p, dir, steps);
            return {};
        case direction::west:
            if (p.x > (bounding_rect.left + steps - 1)) return get_next_pos(p, dir, steps);
            return {};
    };

    std::unreachable();
}

template <bool include_diagonal, typename T>
using neighbours = boost::container::static_vector<aoc::generic_point<T>, include_diagonal ? 8 : 4>;

template <bool include_diagonal = true, typename T>
neighbours<include_diagonal, T> get_neighbours(const T x, const T y, const aoc::generic_rect<T>& bounding_rect)
{
    const auto min_x = bounding_rect.left;
    const auto max_x = bounding_rect.right;
    const auto min_y = bounding_rect.top;
    const auto max_y = bounding_rect.bottom;

    neighbours<include_diagonal, T> neighbours;

    const bool left   = x > min_x;
    const bool right  = x < max_x;
    const bool top    = y > min_y;
    const bool bottom = y < max_y;

#pragma warning(push)
#pragma warning(disable : 4127)

    if (left)
    {
        if (include_diagonal && top) neighbours.emplace_back(x - 1, y - 1);
        neighbours.emplace_back(x - 1, y);
        if (include_diagonal && bottom) neighbours.emplace_back(x - 1, y + 1);
    }

    if (right)
    {
        if (include_diagonal && top) neighbours.emplace_back(x + 1, y - 1);
        neighbours.emplace_back(x + 1, y);
        if (include_diagonal && bottom) neighbours.emplace_back(x + 1, y + 1);
    }

#pragma warning(pop)

    if (top) { neighbours.emplace_back(x, y - 1); }

    if (bottom) { neighbours.emplace_back(x, y + 1); }

    return neighbours;
}

template <bool include_diagonal = true, typename T>
neighbours<include_diagonal, T> get_neighbours(const aoc::generic_point<T> loc,
                                               const aoc::generic_rect<T>& bounding_rect)
{
    return get_neighbours<include_diagonal>(loc.x, loc.y, bounding_rect);
}

template <bool include_diagonal = true, typename T>
neighbours<include_diagonal, T> get_neighbours(const T x, const T y)
{
    const auto t_min = std::numeric_limits<T>::min();
    const auto t_max = std::numeric_limits<T>::max();
    return get_neighbours<include_diagonal>(x, y, { { t_min, t_min }, { t_max, t_max } });
}

template <bool include_diagonal = true, typename T>
neighbours<include_diagonal, T> get_neighbours(const aoc::generic_point<T> loc)
{
    return get_neighbours<include_diagonal>(loc.x, loc.y);
}

}  // namespace aoc

template <typename T>
struct std::hash<aoc::generic_point<T>>
{
    std::size_t operator()(const aoc::generic_point<T>& l) const noexcept
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, l.x);
        boost::hash_combine(seed, l.y);

        return seed;
    }
};
