#pragma once

namespace aoc
{
namespace functors
{
struct min
{
    template <typename T>
    const T& operator()(const T& a, const T& b) const
    {
        return a < b ? a : b;
    }
};

struct max
{
    template <typename T>
    const T& operator()(const T& a, const T& b) const
    {
        return a < b ? b : a;
    }
};
}  // namespace functors

}  // namespace aoc