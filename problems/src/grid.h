#pragma once

#include "coords.h"
#include "ensure.h"
#include "stream.h"

#include <cassert>
#include <istream>
#include <iterator>
#include <optional>
#include <ranges>
#include <string>

namespace aoc
{

template <typename T>
class generic_grid
{
  public:
    generic_grid() : m_width(0), m_height(0) {}
    generic_grid(const size_t width, const size_t height) : m_str(width * height, T{}), m_width(width), m_height(height)
    {
    }
    generic_grid(std::basic_string<T> str, const size_t width, const size_t height) :
        m_str(std::move(str)), m_width(width), m_height(height)
    {
    }

    // TODO: use c++23 multidimensional [] op when available
    T operator()(const size_t x, const size_t y) const
    {
        return m_str.data()[ m_width * y + x ];
    }

    T& operator()(const size_t x, const size_t y)
    {
        return m_str.data()[ m_width * y + x ];
    }

    auto get_range()
    {
        return m_str | std::ranges::views::all;
    }

    auto get_range() const
    {
        return m_str | std::ranges::views::all;
    }

    auto get_col(const size_t x) const
    {
        assert(x < m_width);
        return std::ranges::views::iota(size_t(0), m_height) |
               std::ranges::views::transform([ = ](const auto y) { return (*this)(x, y); });
    }

    auto get_row(const size_t y) const
    {
        assert(y < m_height);
        return std::ranges::views::iota(size_t(0), m_width) |
               std::ranges::views::transform([ = ](const auto x) { return (*this)(x, y); });
    }

    const std::basic_string<T>& str() const
    {
        return m_str;
    }

    size_t get_width() const
    {
        return m_width;
    }

    size_t get_height() const
    {
        return m_height;
    }

    std::optional<aoc::upoint> find(const T t) const
    {
        auto pos = m_str.find(t);
        if (pos == std::basic_string<T>::npos) return {};

        return aoc::upoint{ pos % m_width, pos / m_width };
    }

    static generic_grid<T> read_whole_input(std::istream& input, const size_t reserve = 0)
    {
        std::basic_string<T> grid_str;

        const auto width_opt = aoc::peek_line_elements<T>(input);
        ensure(width_opt);
        const size_t width = *width_opt;

        grid_str.reserve(reserve ? reserve : (width * width));
        grid_str.append(std::istream_iterator<T>(input), std::istream_iterator<T>());

        size_t height = 0;
        if (width != 0)
        {
            assert(grid_str.size() % width == 0);
            height = (grid_str.size() / width);
        }

        return { std::move(grid_str), width, height };
    }

    friend std::istream& operator>>(std::istream& is, generic_grid<T>& grid)
    {
        grid.m_str.clear();
        grid.m_height = 0;
        grid.m_width  = 0;

        bool calc_width = true;
        std::basic_string<char> line;

        while (is.peek() == '\n') { is.get(); }

        while (std::getline(is, line) && !line.empty())
        {
            std::ispanstream ss(line);
            T elem;
            while (ss >> elem)
            {
                grid.m_str.push_back(std::move(elem));
                if (calc_width) ++grid.m_width;
            }

            ++grid.m_height;
            calc_width = false;
        }

        if (!grid.m_str.empty()) { is.clear(); }

        return is;
    }

  private:
    std::basic_string<T> m_str;
    size_t m_width;
    size_t m_height;
};

using grid = generic_grid<char>;

}  // namespace aoc
