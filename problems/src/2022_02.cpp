#include "aoc/problems/2022_02.h"

#include "input.h"

#include <cassert>
#include <iostream>

namespace
{
enum class hand
{
    Rock     = 0,
    Paper    = 1,
    Scissors = 2
};

hand find_hand_to_lose(const hand opponent)
{
    return static_cast<hand>((static_cast<int>(opponent) + 2) % 3);
}

hand find_hand_to_win(const hand opponent)
{
    return static_cast<hand>((static_cast<int>(opponent) + 1) % 3);
}

hand parse_opponent(const char c)
{
    assert(c >= 'A' && c <= 'C');
    return static_cast<hand>(c - 'A');
}

hand parse_player(const char c)
{
    assert(c >= 'X' && c <= 'Z');
    return static_cast<hand>(c - 'X');
}

size_t calc_score(const hand opponent, const hand your)
{
    size_t score = 0;

    if (your == find_hand_to_win(opponent))
        score += 6;
    else if (your == opponent)
        score += 3;

    return score + static_cast<size_t>(your) + 1;
}

size_t calc_game_score1(const hand opponent, const char your_column)
{
    return calc_score(opponent, parse_player(your_column));
}

size_t calc_game_score2(const hand opponent, const char player_column)
{
    switch (player_column)
    {
        case 'X': return calc_score(opponent, find_hand_to_lose(opponent));  // lose
        case 'Y': return calc_score(opponent, opponent);                     // draw
        case 'Z': return calc_score(opponent, find_hand_to_win(opponent));   // win
        default: assert(false); return 0;
    }
}

template <size_t (*calc_score)(hand, char)>
size_t solve(std::istream& input)
{
    size_t totalScore = 0;

    char opponent;
    while (input >> opponent)
    {
        char your_column;
        input >> your_column;

        totalScore += calc_score(parse_opponent(opponent), your_column);
    }

    return totalScore;
}

}  // namespace

namespace aoc
{
namespace year_2022
{

size_t solve02_a(std::istream& input)
{
    return solve<calc_game_score1>(input);
}

size_t solve02_b(std::istream& input)
{
    return solve<calc_game_score2>(input);
}

}  // namespace year_2022
}  // namespace aoc