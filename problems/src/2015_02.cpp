#include "aoc/problems/2015_02.h"

#include "parse.h"

#include <algorithm>
#include <ranges>

#include <boost/fusion/adapted/struct.hpp>

namespace
{
struct dimensions
{
    size_t length, width, height;
};
}  // namespace

BOOST_FUSION_ADAPT_STRUCT(dimensions, length, width, height)

namespace
{
auto make_parser()
{
    namespace x3 = boost::spirit::x3;
    return aoc::x3_size_t_ > 'x' > aoc::x3_size_t_ > 'x' > aoc::x3_size_t_;
}

size_t calc_paper(const size_t l, const size_t w, const size_t h, const size_t max_side)
{
    const size_t base  = 2 * l * w + 2 * w * h + 2 * h * l;
    const size_t extra = (l * w * h) / max_side;

    return base + extra;
}

size_t calc_ribbon(const size_t l, const size_t w, const size_t h, const size_t max_side)
{
    const auto present_ribbon = 2 * ((l + w + h) - max_side);
    const auto bow_ribbon     = l * w * h;

    return present_ribbon + bow_ribbon;
}

template <auto calc_wrapper>
size_t solve(std::istream& input)
{
    const auto dims = aoc::parse_lines<dimensions>(input, make_parser());

    const auto required_materials = dims | std::ranges::views::transform(
                                               [](const dimensions& dims)
                                               {
                                                   const auto [ l, w, h ] = dims;
                                                   const auto max_side    = std::max(std::max(l, w), h);

                                                   return calc_wrapper(l, w, h, max_side);
                                               });

    return std::ranges::fold_left(required_materials, size_t(0), std::plus{});
}

}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve02_a(std::istream& input)
{
    return solve<calc_paper>(input);
}

size_t solve02_b(std::istream& input)
{
    return solve<calc_ribbon>(input);
}

}  // namespace year_2015
}  // namespace aoc
