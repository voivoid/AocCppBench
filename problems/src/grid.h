#pragma once

#include "coords.h"

#include <cassert>
#include <istream>
#include <iterator>
#include <optional>
#include <ranges>
#include <string>

namespace aoc
{

class grid
{
  public:
    grid() : m_width(0), m_height(0) {}
    grid(std::string str, const size_t width, const size_t height) :
        m_str(std::move(str)), m_width(width), m_height(height)
    {
    }

    // TODO: use c++23 multidimensional [] op when available
    char operator()(const size_t x, const size_t y) const
    {
        return m_str.data()[ m_width * y + x ];
    }

    char& operator()(const size_t x, const size_t y)
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

    const std::string& str() const
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

    std::optional<aoc::upoint> find(const char t)
    {
        auto pos = m_str.find(t);
        if (pos == std::string::npos) return {};

        return aoc::upoint{ pos % m_width, pos / m_width };
    }

    static grid read_whole_input(std::istream& input, const size_t reserve = 0)
    {
        std::string grid_str;

        std::string line;
        std::getline(input, line);
        const size_t width = line.size();

        grid_str.reserve(reserve ? reserve : (width * width));

        grid_str.append(line);
        grid_str.append(std::istream_iterator<char>(input), std::istream_iterator<char>());

        size_t height = 0;
        if (width != 0)
        {
            assert(grid_str.size() % width == 0);
            height = (grid_str.size() / width);
        }

        return { std::move(grid_str), width, height };
    }

    friend std::istream& operator>>(std::istream& is, grid& grid)
    {
        grid.m_str.clear();
        grid.m_height = 0;
        grid.m_width  = 0;

        std::string line;
        while (std::getline(is, line) && !line.empty())
        {
            grid.m_str.append(line);

            grid.m_width = line.size();
            ++grid.m_height;
        }

        if (!grid.m_str.empty()) { is.clear(); }

        return is;
    }

  private:
    std::string m_str;
    size_t m_width;
    size_t m_height;
};

}  // namespace aoc
