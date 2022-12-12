#include "aoc/problems/2022_08.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

namespace
{
using height = uint8_t;

struct col
{
    col(const size_t c) : v(c)
    {
    }
    static col make(const size_t c)
    {
        return col{ c };
    }
    size_t v;
};

struct row
{
    row(const size_t r) : v(r)
    {
    }
    static row make(const size_t r)
    {
        return row{ r };
    }
    size_t v;
};

struct heights_grid
{
    std::vector<height> heights;
    size_t cols = 0;
    size_t rows = 0;

    size_t get_height_idx(const size_t col, const size_t row) const
    {
        return cols * row + col;
    }

    size_t get_height_idx(const col col, const row row) const
    {
        return get_height_idx(col.v, row.v);
    }

    size_t get_height_idx(const row row, const col col) const
    {
        return get_height_idx(col.v, row.v);
    }
};

heights_grid parse_input(std::istream& input)
{
    heights_grid grid;
    auto& heights = grid.heights;

    std::string line;
    while (std::getline(input, line))
    {
        if (!grid.cols)
            grid.cols = line.size();

        auto line_heights = line | std::views::transform([](const char c) { return static_cast<uint8_t>(c - '0'); });
        heights.insert(heights.end(), line_heights.begin(), line_heights.end());

        ++grid.rows;
    }

    return grid;
}

struct height_scan
{
    template <typename R1, typename R2>
    static std::vector<height> calc(const heights_grid& grid, R1 r1, R2 r2)
    {
        const auto total_trees = grid.cols * grid.rows;

        std::vector<height> scan(total_trees);
        for (const auto i1 : r1)
        {
            height max = 0;
            for (const auto i2 : r2)
            {
                const auto height_idx = grid.get_height_idx(i1, i2);

                const auto grid_height = grid.heights[ height_idx ];
                max                    = std::max(max, grid_height);
                scan[ height_idx ]     = max;
            }
        }

        return scan;
    }
};

struct visible_trees_scan
{
    template <typename R1, typename R2>
    static std::vector<size_t> calc(const heights_grid& grid, R1 r1, R2 r2)
    {
        const auto total_trees = grid.cols * grid.rows;

        std::vector<size_t> scan(total_trees);
        for (const auto i1 : r1)
        {
            std::map<height, size_t> prev_map;

            size_t neighbours = 0;
            std::optional<height> prev;
            for (const auto i2 : r2)
            {
                const auto height_idx = grid.get_height_idx(i1, i2);

                const auto grid_height = grid.heights[ height_idx ];

                if (!prev)
                    prev = grid_height;
                else if (prev < grid_height)
                {
                    const auto lower_keys = prev_map | std::views::keys |
                                            std::views::filter([ grid_height ](const auto k) { return k < grid_height; }) |
                                            std::ranges::to<std::vector>();
                    for (const auto k : lower_keys)
                    {
                        auto iter = prev_map.find(k);
                        neighbours += iter->second;
                        prev_map.erase(iter);
                    }

                    ++neighbours;
                }
                else
                {
                    assert(prev >= grid_height);

                    auto& v = prev_map[ *prev ];
                    v += neighbours;

                    neighbours = 1;
                }

                scan[ height_idx ] = neighbours;
                prev               = grid_height;
            }
        }

        return scan;
    }
};

bool is_visible(const size_t col,
                const size_t row,
                const heights_grid& grid,
                const std::vector<height>& left_to_right_scan,
                const std::vector<height>& right_to_left_scan,
                const std::vector<height>& up_to_down_scan,
                const std::vector<height>& down_to_up_scan)
{
    if ((col == 0) || (row == 0) || (col == (grid.cols - 1)) || (row == (grid.rows - 1)))
        return true;

    const auto height_idx = grid.get_height_idx(col, row);
    const auto height     = grid.heights[ height_idx ];

    const auto left_idx        = grid.get_height_idx(col - 1, row);
    const auto left_max_height = left_to_right_scan[ left_idx ];
    if (height > left_max_height)
        return true;

    const auto right_idx        = grid.get_height_idx(col + 1, row);
    const auto right_max_height = right_to_left_scan[ right_idx ];
    if (height > right_max_height)
        return true;

    const auto up_idx        = grid.get_height_idx(col, row - 1);
    const auto up_max_height = up_to_down_scan[ up_idx ];
    if (height > up_max_height)
        return true;

    const auto down_idx        = grid.get_height_idx(col, row + 1);
    const auto down_max_height = down_to_up_scan[ down_idx ];
    if (height > down_max_height)
        return true;

    return false;
}

size_t calc_scenic_score(const size_t col,
                         const size_t row,
                         const heights_grid& grid,
                         const std::vector<size_t>& left_to_right_scan,
                         const std::vector<size_t>& right_to_left_scan,
                         const std::vector<size_t>& up_to_down_scan,
                         const std::vector<size_t>& down_to_up_scan)
{
    const auto idx         = grid.get_height_idx(col, row);
    const auto left_trees  = left_to_right_scan[ idx ];
    const auto right_trees = right_to_left_scan[ idx ];
    const auto up_trees    = up_to_down_scan[ idx ];
    const auto down_trees  = down_to_up_scan[ idx ];

    return left_trees * right_trees * up_trees * down_trees;
}


template <typename Scan, typename Ret, typename... Args>
std::vector<Ret> solve(std::istream& input, Ret (*calc)(Args...))
{
    const heights_grid grid = parse_input(input);

    const auto cols          = std::views::iota(size_t(0), grid.cols) | std::views::transform(col::make);
    const auto cols_reversed = cols | std::views::reverse;
    const auto rows          = std::views::iota(size_t(0), grid.rows) | std::views::transform(row::make);
    const auto rows_reversed = rows | std::views::reverse;

    const auto left_to_right_scan = Scan::calc(grid, rows, cols);
    const auto right_to_left_scan = Scan::calc(grid, rows, cols_reversed);
    const auto up_to_down_scan    = Scan::calc(grid, cols, rows);
    const auto down_to_up_scan    = Scan::calc(grid, cols, rows_reversed);

    std::vector<Ret> result;
    result.reserve(grid.cols * grid.rows);
    for (const auto col : cols)
    {
        for (const auto row : rows)
        {
            result.push_back(calc(col.v, row.v, grid, left_to_right_scan, right_to_left_scan, up_to_down_scan, down_to_up_scan));
        }
    }

    return result;
}

}  // namespace

namespace aoc
{
namespace year_2022
{

size_t solve08_a(std::istream& input)
{
    const std::vector<bool> trees_visibility = solve<height_scan>(input, is_visible);
    return std::count(trees_visibility.cbegin(), trees_visibility.cend(), true);
}

size_t solve08_b(std::istream& input)
{
    const std::vector<size_t> scores = solve<visible_trees_scan>(input, calc_scenic_score);
    return *std::max_element(scores.begin(), scores.end());
}

}  // namespace year_2022
}  // namespace aoc