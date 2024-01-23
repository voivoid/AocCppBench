#include "aoc/problems/2015_16.h"

#include "fusion.h"
#include "parse.h"

#include <algorithm>
#include <istream>
#include <tuple>
#include <ranges>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/adapted/struct.hpp>
#include <boost/unordered/unordered_flat_map.hpp>

namespace
{
enum class compound
{
    children,
    cats,
    samoyeds,
    pomeranians,
    akitas,
    vizslas,
    goldfish,
    trees,
    cars,
    perfumes
};

struct aunt_compounds
{
    size_t idx;
    boost::unordered_flat_map<compound, size_t> compounds;
};

aunt_compounds get_the_aunt()
{
    using enum compound;
    aunt_compounds the_aunt;
    the_aunt.compounds[ children ]    = 3;
    the_aunt.compounds[ cats ]        = 7;
    the_aunt.compounds[ samoyeds ]    = 2;
    the_aunt.compounds[ pomeranians ] = 3;
    the_aunt.compounds[ akitas ]      = 0;
    the_aunt.compounds[ vizslas ]     = 0;
    the_aunt.compounds[ goldfish ]    = 5;
    the_aunt.compounds[ trees ]       = 3;
    the_aunt.compounds[ cars ]        = 2;
    the_aunt.compounds[ perfumes ]    = 1;

    return the_aunt;
}

}  // namespace

BOOST_FUSION_ADAPT_STRUCT(aunt_compounds, idx, compounds)

namespace
{

auto make_parser()
{
    namespace x3 = boost::spirit::x3;

    x3::symbols<compound> compounds_parser = {
        { "children", compound::children },       { "cats", compound::cats },     { "samoyeds", compound::samoyeds },
        { "pomeranians", compound::pomeranians }, { "akitas", compound::akitas }, { "vizslas", compound::vizslas },
        { "goldfish", compound::goldfish },       { "trees", compound::trees },   { "cars", compound::cars },
        { "perfumes", compound::perfumes }
    };

    const auto param = x3::rule<struct _params, std::pair<compound, size_t>>{} =
        compounds_parser > ':' > aoc::x3_size_t_;
    const auto params = param % ',';

    const auto parser = x3::rule<struct _aunt, ::aunt_compounds>{} = x3::lit("Sue") > aoc::x3_size_t_ > ':' > params;
    return parser;
}

bool match_compound_1(const size_t some_aunt_compound, const size_t the_aunt_compound, const compound)
{
    return some_aunt_compound == the_aunt_compound;
}

bool match_compound_2(const size_t some_aunt_compound, const size_t the_aunt_compound, const compound compound)
{
    if (compound == compound::cats || compound == compound::trees) return some_aunt_compound > the_aunt_compound;
    else if (compound == compound::pomeranians || compound == compound::goldfish)
        return some_aunt_compound < the_aunt_compound;
    else
        return some_aunt_compound == the_aunt_compound;
}

template <bool match_compound(size_t, size_t, compound)>
bool is_the_aunt(const aunt_compounds& some_aunt)
{
    static const auto the_aunt = get_the_aunt();

    for (const auto [ compound, param ] : some_aunt.compounds)
    {
        const auto the_aunt_compound_iter = the_aunt.compounds.find(compound);
        assert(the_aunt_compound_iter != the_aunt.compounds.cend());

        if (!match_compound(param, the_aunt_compound_iter->second, compound)) return false;
    }

    return true;
}

template <bool match_compound(size_t, size_t, compound)>
size_t solve(std::istream& input)
{
    const auto aunts = aoc::parse_lines<aunt_compounds>(input, make_parser());
    auto aunt_iter = std::ranges::find_if(aunts, is_the_aunt<match_compound>);
    assert(aunt_iter != aunts.cend());

    return aunt_iter->idx;
}

}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve16_a(std::istream& input)
{
    return solve<match_compound_1>(input);
}

size_t solve16_b(std::istream& input)
{
    return solve<match_compound_2>(input);
}

}  // namespace year_2015
}  // namespace aoc
