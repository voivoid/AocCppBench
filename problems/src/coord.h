#pragma once

#include <array>
#include <functional>

#include <boost/functional/hash.hpp>

namespace aoc
{

template <typename T>
struct coord
{
    T x;
    T y;
};

using pos = coord<size_t>;

template <typename T>
bool operator==(const coord<T>& p1, const coord<T>& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

template <typename T>
bool operator!=(const coord<T>& p1, const coord<T>& p2)
{
    return !(p1 == p2);
}

}  // namespace aoc

template <typename T>
struct std::hash<aoc::coord<T>>
{
    std::size_t operator()(const aoc::pos& p) const noexcept
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, p.x);
        boost::hash_combine(seed, p.y);

        return seed;
    }
};