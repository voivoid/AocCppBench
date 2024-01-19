#include "aoc/problems/2021_05.h"

#include "coords.h"
#include "coords_fusion.h"
#include "ensure.h"
#include "parse.h"
#include "views.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/segment.hpp>

#include <istream>
#include <ranges>
#include <unordered_set>
#include <vector>


BOOST_GEOMETRY_REGISTER_POINT_2D(aoc::point, long long, cs::cartesian, x, y);
BOOST_GEOMETRY_REGISTER_SEGMENT(aoc::line, aoc::point, from, to);

namespace
{

aoc::line::dir get_line_dir(const aoc::line& line)
{
    const auto& [ p1, p2 ] = line;

    if (p1.x == p2.x) return aoc::line::dir::vertical;
    if (p1.y == p2.y) return aoc::line::dir::horizontal;
    return aoc::line::dir::diagonal;
}

auto make_line_parser()
{
    namespace x3 = boost::spirit::x3;

    static const auto line_action = [](const auto& ctx)
    {
        const auto [ p1, p2 ] = aoc::x3_attrs_tuple(ctx);

        aoc::line& val = x3::_val(ctx);

        std::tie(val.from, val.to) = std::minmax(p1, p2);
    };

    auto point_parser = x3::rule<struct _point, aoc::point>{} = x3::int_ > ',' > x3::int_;
    auto line_parser = x3::rule<struct _line, aoc::line>{} = (point_parser > "->" > point_parser)[ line_action ];
    return line_parser;
}

size_t solve(std::istream& input, const bool ignore_diagonal)
{
    using enum aoc::line::dir;
    namespace views = std::ranges::views;

    auto lines = aoc::parse_lines<aoc::line>(input, make_line_parser()) | std::ranges::to<std::vector<aoc::line>>();
    std::ranges::sort(lines);

    std::unordered_set<aoc::point> all_intersections;

    std::vector<aoc::point> line_intersections;
    for (auto l1_iter = lines.cbegin(); l1_iter != lines.cend(); ++l1_iter)
    {
        const auto& l1 = *l1_iter;

        const auto l1_dir = get_line_dir(l1);
        if (ignore_diagonal && l1_dir == diagonal) continue;

        const auto rest       = std::ranges::subrange(std::next(l1_iter), lines.cend());
        const auto l1_bounded = std::ranges::subrange(std::ranges::lower_bound(rest, l1.from, {}, &aoc::line::from),
                                                      std::ranges::upper_bound(rest, l1.to, {}, &aoc::line::from));

        for (const auto& l2 : l1_bounded)
        {
            const auto l2_dir = get_line_dir(l2);
            if (ignore_diagonal && l2_dir == diagonal) continue;

            boost::geometry::intersection(l1, l2, line_intersections);

            if (line_intersections.size() == 1)
            {
                // boost geometry has some issues with integer arithmetic
                // so double-checking is needed for diagonal lines
                if ((l1_dir != diagonal && l2_dir != diagonal) ||
                    (boost::geometry::intersects(l1, line_intersections[ 0 ]) &&
                     boost::geometry::intersects(l2, line_intersections[ 0 ])))
                {
                    all_intersections.insert(line_intersections[ 0 ]);
                }
            }
            else if (line_intersections.size() == 2)
            {
                const auto& from = line_intersections[ 0 ];
                const auto& to   = line_intersections[ 1 ];

                if (l1_dir == horizontal)
                {
                    assert(l2_dir == horizontal);
                    const auto [ x_from, x_to ] = std::minmax(from.x, to.x);
                    all_intersections.insert_range(views::iota(x_from, x_to + 1) | views::transform(
                                                                                       [ y = from.y ](const auto x) {
                                                                                           return aoc::point{ x, y };
                                                                                       }));
                }
                else if (l1_dir == vertical)
                {
                    assert(l2_dir == vertical);
                    const auto [ y_from, y_to ] = std::minmax(from.y, to.y);
                    all_intersections.insert_range(views::iota(y_from, y_to + 1) | views::transform(
                                                                                       [ x = from.x ](const auto y) {
                                                                                           return aoc::point{ x, y };
                                                                                       }));
                }
                else
                {
                    assert(l1_dir == diagonal);
                    assert(l2_dir == diagonal);

                    auto xs = aoc::nums_view(from.x, to.x);
                    auto ys = aoc::nums_view(from.y, to.y);

                    all_intersections.insert_range(views::zip_transform(
                        [](const auto x, const auto y) {
                            return aoc::point{ x, y };
                        },
                        xs,
                        ys));

                    const auto [ x_from, x_to ] = std::minmax(from.x, to.x);
                    const auto [ y_from, y_to ] = std::minmax(from.y, to.y);
                    assert(x_to - x_from == y_to - y_from);
                }
            }
            else { assert(line_intersections.empty()); }

            line_intersections.clear();
        }
    }

    return all_intersections.size();
}

}  // namespace

namespace aoc
{
namespace year_2021
{

size_t solve05_a(std::istream& input)
{
    return solve(input, true);
}

size_t solve05_b(std::istream& input)
{
    return solve(input, false);
}

}  // namespace year_2021
}  // namespace aoc
