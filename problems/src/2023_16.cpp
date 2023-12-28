#include "aoc/problems/2023_16.h"

#include "coords.h"
#include "grid.h"

#include <array>
#include <cstddef>
#include <istream>
#include <mdspan>
#include <ranges>

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm.hpp>

namespace
{

struct beam_state
{
    aoc::upoint pos;
    aoc::direction dir;
};

class tile
{
  public:
    tile(const char c) : m_chr(char_to_byte(c)), m_dir(0), m_out(0) {}

    char ch() const
    {
        return char_map[ m_chr ];
    }

    bool is_visited() const
    {
        return m_dir != 0;
    }

    bool is_visited(const aoc::direction dir) const
    {
        const auto dm = dir_mask(dir);
        return (m_dir & dm) == dm;
    }

    void visit(const aoc::direction dir)
    {
        assert(!is_visited(dir));
        m_dir |= dir_mask(dir);
        assert(is_visited(dir));
    }

    void clear_visit()
    {
        m_dir = 0;
    }

    void set_out()
    {
        m_out = 1;
    }

    bool is_out() const
    {
        return m_out != 0;
    }

    bool operator==(const char c) const
    {
        return ch() == c;
    }

  private:
    static constexpr std::array<char, 5> char_map{ '.', '-', '|', '/', '\\' };

    static uint8_t char_to_byte(const char c)
    {
        auto iter = std::find(char_map.cbegin(), char_map.cend(), c);
        assert(iter != char_map.cend());

        return static_cast<uint8_t>(iter - char_map.cbegin());
    }

    static uint8_t dir_mask(const aoc::direction dir)
    {
        return 1 << static_cast<uint8_t>(dir);
    }

  private:
    uint8_t m_chr : 3;
    uint8_t m_dir : 4;
    uint8_t m_out : 1;
};

struct layout
{
    std::vector<tile> tiles;
    size_t width;
    size_t height;
};


void clear_layout(layout& layout)
{
    for (auto& tile : layout.tiles) { tile.clear_visit(); }
}

layout read_layout(std::istream& input)
{
    // TODO: make aoc::grid generic
    const auto grid = aoc::grid::read_whole_input(input);

    layout layout;
    layout.tiles.assign(grid.str().cbegin(), grid.str().cend());

    layout.width  = grid.get_width();
    layout.height = grid.get_height();

    return layout;
}

std::tuple<std::optional<aoc::direction>, std::optional<aoc::direction>> get_next_dirs(const aoc::direction current_dir,
                                                                                       const tile current_tile)
{
    switch (current_tile.ch())
    {
        case '.': return { current_dir, {} };
        case '-':
        {
            if (current_dir == aoc::direction::east || current_dir == aoc::direction::west)
            {
                return { current_dir, {} };
            }
            return { aoc::direction::east, aoc::direction::west };
        }

        case '|':
        {
            if (current_dir == aoc::direction::north || current_dir == aoc::direction::south)
            {
                return { current_dir, {} };
            }
            return { aoc::direction::north, aoc::direction::south };
        }

        case '/':
        {
            switch (current_dir)
            {
                case aoc::direction::north: return { aoc::direction::east, {} };
                case aoc::direction::south: return { aoc::direction::west, {} };
                case aoc::direction::east: return { aoc::direction::north, {} };
                case aoc::direction::west: return { aoc::direction::south, {} };
            }
        }
        case '\\':
            switch (current_dir)
            {
                case aoc::direction::north: return { aoc::direction::west, {} };
                case aoc::direction::south: return { aoc::direction::east, {} };
                case aoc::direction::east: return { aoc::direction::south, {} };
                case aoc::direction::west: return { aoc::direction::north, {} };
            }
    }

    std::unreachable();
}

size_t calc_configuration_energy(layout& layout, const beam_state initial_state)
{
    std::mdspan mdspan(layout.tiles.data(), layout.width, layout.height);

    const auto right         = layout.width - 1;
    const auto bottom        = layout.height - 1;
    const auto bounding_rect = aoc::rect{ { 0, 0 }, { right, bottom } };

    std::vector<beam_state> states{ initial_state };
    while (!states.empty())
    {
        const auto state = states.back();
        states.pop_back();

        tile& current_tile = mdspan[ std::array{ state.pos.y, state.pos.x } ];

        if (current_tile.is_visited(state.dir)) continue;

        current_tile.visit(state.dir);

        const bool is_horizontal_out = (current_tile == '.' || current_tile == '-') &&
                                       ((state.pos.x == 0 && state.dir == aoc::direction::west) ||
                                        (state.pos.x == right && state.dir == aoc::direction::east));
        const bool is_vertical_out = (current_tile == '.' || current_tile == '|') &&
                                     ((state.pos.y == 0 && state.dir == aoc::direction::north) ||
                                      (state.pos.y == bottom && state.dir == aoc::direction::south));

        if (is_horizontal_out || is_vertical_out) { current_tile.set_out(); }

        const auto next_dirs = get_next_dirs(state.dir, current_tile);
        boost::fusion::for_each(next_dirs,
                                [ & ](const auto& next_dir)
                                {
                                    if (!next_dir) return;

                                    auto next_pos = aoc::get_next_pos(state.pos, *next_dir, bounding_rect);
                                    if (!next_pos) return;

                                    states.emplace_back(*next_pos, *next_dir);
                                });
    }

    return std::ranges::count_if(layout.tiles, [](const auto tile) { return tile.is_visited(); });
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve16_a(std::istream& input)
{
    auto layout = read_layout(input);
    const beam_state initial_state{ { 0, 0 }, aoc::direction::east };

    return calc_configuration_energy(layout, initial_state);
}

size_t solve16_b(std::istream& input)
{
    auto layout       = read_layout(input);
    const auto width  = layout.width;
    const auto height = layout.height;
    std::mdspan mdspan(layout.tiles.data(), width, height);

    size_t max_energy = 0;
    const auto calc   = [ & ](const beam_state initial_state)
    {
        const auto initial_tile = mdspan[ std::array{ initial_state.pos.y, initial_state.pos.x } ];
        if (initial_tile.is_out()) return;

        max_energy = std::max(max_energy, calc_configuration_energy(layout, initial_state));
        clear_layout(layout);
    };

    // TODO: use c++26 views::concat when it'll available
    for (size_t x = 0; x < width; ++x) { calc({ { x, 0 }, aoc::direction::south }); }
    for (size_t y = 0; y < height; ++y) { calc({ { 0, y }, aoc::direction::east }); }
    for (size_t y = 0; y < height; ++y) { calc({ { width - 1, y }, aoc::direction::west }); }
    for (size_t x = 0; x < width; ++x) { calc({ { x, height - 1 }, aoc::direction::north }); }

    return max_energy;
}

}  // namespace year_2023
}  // namespace aoc
