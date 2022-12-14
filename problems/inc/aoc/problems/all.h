#pragma once

#include <iosfwd>
#include <optional>
#include <string>
#include <unordered_map>

namespace aoc
{
using solver_f = void (*)(std::istream&, std::ostream&);

struct problem
{
    solver_f solver;
    size_t year;
    size_t day;
};

struct problem_registrator
{
    problem_registrator(std::string problem_name, solver_f f, size_t year, size_t day)
    {
        problems.emplace(std::move(problem_name), aoc::problem{ f, year, day });
    }

    static std::optional<problem> get_registered_problem(const std::string& problem_name)
    {
        auto problem_iter = problems.find(problem_name);
        if (problem_iter == problems.cend())
            return {};

        return { problem_iter->second };
    }

    template <auto F>
    static void adapt_to_ostream(std::istream& in, std::ostream& out)
    {
        out << F(in);
    }

    static std::unordered_map<std::string, problem> problems;
};
}  // namespace aoc

std::unordered_map<std::string, aoc::problem> aoc::problem_registrator::problems;
#define AOC_REGISTER_PROBLEM(PROBLEM_ID, FUNCTION, YEAR, DAY)                                                                              \
    namespace aoc                                                                                                                          \
    {                                                                                                                                      \
    problem_registrator p_##PROBLEM_ID{ #PROBLEM_ID, &problem_registrator::adapt_to_ostream<&FUNCTION>, size_t{ YEAR }, size_t{ DAY } };   \
    }

#include "aoc/problems/2021_01.h"
#include "aoc/problems/2021_02.h"
#include "aoc/problems/2021_03.h"
#include "aoc/problems/2021_04.h"
#include "aoc/problems/2022_01.h"
#include "aoc/problems/2022_02.h"
#include "aoc/problems/2022_03.h"
#include "aoc/problems/2022_04.h"
#include "aoc/problems/2022_05.h"
#include "aoc/problems/2022_06.h"
#include "aoc/problems/2022_07.h"
#include "aoc/problems/2022_08.h"
#include "aoc/problems/2022_09.h"
#include "aoc/problems/2022_10.h"
#include "aoc/problems/2022_11.h"
#include "aoc/problems/2022_12.h"
#include "aoc/problems/2022_13.h"
#include "aoc/problems/2022_14.h"
