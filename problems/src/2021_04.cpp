#include "aoc/problems/2021_04.h"

#include "coords.h"
#include "ensure.h"
#include "parse.h"

#include <array>
#include <list>
#include <numeric>
#include <ranges>
#include <string>
#include <unordered_map>

namespace
{
using number = size_t;

struct board
{
    std::unordered_map<number, aoc::generic_point<number>> nums_position;
    std::array<size_t, 5> col_matches = {};
    std::array<size_t, 5> row_matches = {};
};

using boards     = std::list<board>;
using board_iter = boards::iterator;

std::vector<number> parse_random_numbers(std::istream& input)
{
    std::string numbers_str;
    std::getline(input, numbers_str);

    std::vector<number> parsed_nums;
    ensure(aoc::x3_parse(
        numbers_str.cbegin(), numbers_str.cend(), aoc::x3_size_t_ % ',', boost::spirit::x3::space, parsed_nums));

    return parsed_nums;
}

std::istream& operator>>(std::istream& input, board& board)
{
    for (size_t col = 0; col < 5; ++col)
    {
        for (size_t row = 0; row < 5; ++row)
        {
            number num;
            input >> num;

            if (!input) return input;

            auto& pos = board.nums_position[ num ];
            pos.x     = col;
            pos.y     = row;
        }
    }

    return input;
}

boards parse_boards(std::istream& input)
{
    boards boards;

    board board;
    while (input >> board) { boards.push_back(std::move(board)); }

    return boards;
}

bool is_winner(const board& board)
{
    return std::ranges::contains(board.col_matches, 5) || std::ranges::contains(board.row_matches, 5);
}

size_t sum_unmarked(const board& board)
{
    const auto unmarked = board.nums_position | std::views::keys;
    return std::accumulate(unmarked.begin(), unmarked.end(), size_t(0));
}

size_t solve(std::istream& input, const bool find_first_winner)
{
    const auto random_numbers = parse_random_numbers(input);
    auto boards               = parse_boards(input);

    for (const auto drawn_num : random_numbers)
    {
        for (auto board_iter = boards.begin(); board_iter != boards.end();)
        {
            auto& board = *board_iter;

            auto num_pos_iter = board.nums_position.find(drawn_num);
            if (num_pos_iter != board.nums_position.cend())
            {
                const auto col = num_pos_iter->second.x;
                const auto row = num_pos_iter->second.y;

                board.nums_position.erase(num_pos_iter);

                ++board.col_matches[ col ];
                ++board.row_matches[ row ];

                if (is_winner(board))
                {
                    if (find_first_winner || boards.size() == 1) return drawn_num * sum_unmarked(board);
                    board_iter = boards.erase(board_iter);
                    continue;
                }
            }

            ++board_iter;
        }
    }

    std::unreachable();
}

}  // namespace

namespace aoc
{

namespace year_2021
{


size_t solve04_a(std::istream& input)
{
    return solve(input, true);
}

size_t solve04_b(std::istream& input)
{
    return solve(input, false);
}

}  // namespace year_2021
}  // namespace aoc
