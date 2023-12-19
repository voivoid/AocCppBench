#pragma once

#include <algorithm>
#include <functional>

#include <boost/container/static_vector.hpp>
#include <boost/functional/hash.hpp>

namespace aoc
{

template <typename Coord>
struct generic_point
{
    Coord x;
    Coord y;
};

template <typename T>
struct generic_rect
{
    generic_rect(generic_point<T> left_top, generic_point<T> right_bottom) :
        left(left_top.x), right(right_bottom.x), top(left_top.y), bottom(right_bottom.y)
    {
    }

    T left;
    T right;
    T top;
    T bottom;
};

using upoint = generic_point<size_t>;
using point  = generic_point<int>;
using rect   = generic_rect<size_t>;

template <typename T>
bool operator<(const generic_point<T>& l1, const generic_point<T>& l2)
{
    return l1.x != l2.x ? l1.x < l2.x : l1.y < l2.y;
}

template <typename T>
bool operator==(const generic_point<T>& l1, const generic_point<T>& l2)
{
    return l1.x == l2.x && l1.y == l2.y;
}

template <typename T>
bool operator!=(const generic_point<T>& l1, const generic_point<T>& l2)
{
    return !(l1 == l2);
}

template <typename T>
size_t manhattan_distance(const aoc::generic_point<T>& l1, const aoc::generic_point<T>& l2)
{
    const auto [ xmin, xmax ] = std::minmax(l1.x, l2.x);
    const auto [ ymin, ymax ] = std::minmax(l1.y, l2.y);
    return (xmax - xmin) + (ymax - ymin);
}

template <bool include_diagonal, typename T>
using neighbours = boost::container::static_vector<generic_point<T>, include_diagonal ? 8 : 4>;

template <bool include_diagonal = true, typename T>
neighbours<include_diagonal, T> get_neighbours(const T x, const T y, const generic_rect<T>& bounding_rect)
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
neighbours<include_diagonal, T> get_neighbours(const generic_point<T> loc, const generic_rect<T>& bounding_rect)
{
    return get_neighbours<include_diagonal>(loc.x, loc.y, bounding_rect);
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