#pragma once

#include <boost/fusion/adapted/struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(aoc::upoint, x, y);
BOOST_FUSION_ADAPT_STRUCT(aoc::point, x, y);
BOOST_FUSION_ADAPT_STRUCT(aoc::uline, from, to);
BOOST_FUSION_ADAPT_STRUCT(aoc::line, from, to);