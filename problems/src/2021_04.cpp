#include "aoc/problems/2021_04.h"

#include "ensure.h"
#include "input.h"
#include "parse.h"

#include <array>
#include <iostream>
#include <list>
#include <numeric>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace
{
using number = size_t;
struct pos
{
    size_t col;
    size_t row;
};

struct board
{
    std::unordered_map<number, pos> nums_position;
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
    ensure(aoc::x3_parse(numbers_str.cbegin(), numbers_str.cend(), aoc::x3_size_t_ % ',', boost::spirit::x3::space, parsed_nums));

    return parsed_nums;
}

boards parse_boards(std::istream& input)
{
    boards boards;

    while (input.peek() != std::char_traits<char>::eof())
    {
        board board;
        for (size_t col = 0; col < 5; ++col)
        {
            for (size_t row = 0; row < 5; ++row)
            {
                number num;
                input >> num;

                auto& pos = board.nums_position[ num ];
                pos.col   = col;
                pos.row   = row;
            }
        }

        boards.push_back(std::move(board));

        aoc::skip_eol(input);
    }

    return boards;
}

bool is_winner(const board& board)
{
    return std::ranges::contains(board.col_matches, 5) || std::ranges::contains(board.row_matches, 5);
}

std::vector<board_iter> mark_number(const number num, boards& boards)
{
    std::vector<board_iter> winners;

    for (auto iter = boards.begin(); iter != boards.end(); ++iter)
    {
        auto& board   = *iter;
        auto num_iter = board.nums_position.find(num);
        if (num_iter == board.nums_position.cend())
            continue;

        const auto col = num_iter->second.col;
        const auto row = num_iter->second.row;

        board.nums_position.erase(num_iter);

        ++board.col_matches[ col ];
        ++board.row_matches[ row ];

        if (is_winner(board))
        {
            winners.push_back(iter);
        }
    }

    return winners;
}

size_t sum_unmarked(const board& board)
{
    const auto unmarked = board.nums_position | std::views::keys;
    return std::accumulate(unmarked.begin(), unmarked.end(), size_t(0));
}
}  // namespace

namespace aoc
{
namespace year_2021
{

size_t solve04_a(std::istream& input)
{
    const auto random_numbers = parse_random_numbers(input);
    auto boards               = parse_boards(input);

    for (const auto num : random_numbers)
    {
        const auto winners = mark_number(num, boards);
        if (!winners.empty())
        {
            return num * sum_unmarked(*winners.front());
        }
    }

    throw std::runtime_error("unexpected");
}

size_t solve04_b(std::istream& input)
{
    const auto random_numbers = parse_random_numbers(input);
    auto boards               = parse_boards(input);

    for (const auto num : random_numbers)
    {
        const auto winners = mark_number(num, boards);
        for (const auto winner_iter : winners)
        {
            if (boards.size() == 1)
                return num * sum_unmarked(*winner_iter);
            else
            {
                boards.erase(winner_iter);
            }
        }
    }

    throw std::runtime_error("unexpected");
}

}  // namespace year_2021
}  // namespace aoc