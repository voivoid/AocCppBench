#include "aoc/problems/2022_06.h"

#include <iostream>
#include <ranges>
#include <set>
#include <string>

namespace
{
template <typename Slide>
bool are_all_unique(const Slide& slide)
{
    return std::set<char>(slide.begin(), slide.end()).size() == slide.size();
}

size_t solve(std::istream& input, const size_t slide_len)
{
    std::string line;
    std::getline(input, line);

    auto slides          = line | std::views::slide(slide_len);
    auto slide_iter      = std::ranges::find_if(slides, [](const auto& slide) { return are_all_unique(slide); });
    auto marker_distance = std::distance(slides.begin(), slide_iter) + slide_len;

    return marker_distance;
}
}  // namespace

namespace aoc
{
namespace year_2022
{

size_t solve06_a(std::istream& input)
{
    return solve(input, 4);
}

size_t solve06_b(std::istream& input)
{
    return solve(input, 14);
}

}  // namespace year_2022
}  // namespace aoc