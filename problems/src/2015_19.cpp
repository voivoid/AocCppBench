#include "aoc/problems/2015_19.h"

#include "ensure.h"
#include "parse.h"

#include <algorithm>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <boost/algorithm/string/replace.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/adapted/struct.hpp>

namespace
{
using replacemenets_map = std::unordered_multimap<std::string, std::string>;

template <bool reverse_map>
auto parse_input(std::istream& input)
{
    namespace x3 = boost::spirit::x3;

    const auto molecule = x3::upper >> -x3::lower;
    const auto src = x3::rule<struct _src, std::string>{} = x3::string("e") | molecule;
    const auto dest = x3::rule<struct _dest, std::string>{} = x3::lexeme[ +molecule ];

    const auto replacement_action = [](const auto& ctx)
    {
        auto [ left, right ] = aoc::fusion_to_std_tuple(x3::_attr(ctx));
        auto& val            = x3::_val(ctx);

        val.first  = reverse_map ? std::move(right) : std::move(left);
        val.second = reverse_map ? std::move(left) : std::move(right);
    };

    const auto replacement = x3::rule<struct _pair, std::pair<std::string, std::string>>{} =
        (src >> x3::lit("=>") > dest)[ replacement_action ];
    const auto replacements = +replacement;

    input.unsetf(std::ios::skipws);

    boost::spirit::istream_iterator input_iter(input);
    auto replacemenets_map = aoc::x3_parse_attr<::replacemenets_map>(input_iter, replacements, x3::space);
    auto medicine_molecule = aoc::x3_parse_attr<std::string>(input_iter, dest, x3::space);
    ensure(replacemenets_map);
    ensure(medicine_molecule);

    return std::make_pair(std::move(*replacemenets_map), std::move(*medicine_molecule));
}
}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve19_a(std::istream& input)
{
    const auto& [ replacemenets_map, medicine_molecule ] = parse_input<false>(input);

    std::unordered_set<std::string> distinct_molecules;

    const auto sz = medicine_molecule.size();
    for (size_t i = 0; i < sz; ++i)
    {
        const char c1 = medicine_molecule[ i ];
        const char c2 = i + 1 < sz ? medicine_molecule[ i + 1 ] : '\0';

        auto molecule_to_replace = std::islower(c2) ? std::string({ c1, c2 }) : std::string({ c1 });

        auto [ repl_begin, repl_end ] = replacemenets_map.equal_range(molecule_to_replace);
        if (repl_begin == replacemenets_map.cend()) continue;

        for (auto repl_iter = repl_begin; repl_iter != repl_end; ++repl_iter)
        {
            auto new_medicine_molecule = medicine_molecule;
            new_medicine_molecule.replace(i, molecule_to_replace.size(), repl_iter->second);

            distinct_molecules.insert(std::move(new_medicine_molecule));
        }
    }

    return distinct_molecules.size();
}

size_t solve19_b(std::istream& input)
{
    static const std::string_view destination = "e";

    auto [ replacemenets_map, medicine_molecule ] = parse_input<true>(input);

    return 0;
}

}  // namespace year_2015
}  // namespace aoc
