#include <benchmark/benchmark.h>

#include "aoc/problems/all.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace
{

class NullBuffer : public std::streambuf
{
  public:
    int overflow(int c)
    {
        return c;
    }
};

class NullStream : public std::ostream
{
  public:
    NullStream() : std::ostream(&m_sb) {}

  private:
    NullBuffer m_sb;
};

NullStream devnull;

std::string read_problem_input(const size_t year, const size_t day)
{
#ifndef AOC_TEST_INPUT_DIR
#    error AOC_TEST_INPUT_DIR is not defined
#endif

    std::stringstream ss;
    ss << AOC_TEST_INPUT_DIR << '/' << year << '/' << std::setfill('0') << std::setw(2) << day << ".txt";

    const auto input_file = ss.str();

    std::ifstream fs(input_file, std::ios::in);
    if (!fs) { throw std::runtime_error("Failed to open " + input_file); }

    std::string input = std::string((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    return input;
}

void test(benchmark::State& state, const aoc::problem& problem)
{
    std::string input = read_problem_input(problem.year, problem.day);

    for (auto _ : state)
    {
        state.PauseTiming();
        std::stringstream buff(input);
        state.ResumeTiming();

        problem.solver(buff, devnull);
    }
}

}  // namespace

int main(int argc, char** argv)
{
    for (const auto& [ problem_id, problem ] : aoc::problem_registrator::problems)
    {
        ::benchmark::RegisterBenchmark(problem_id.c_str(), test, problem);
    }

    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();

    return 0;
}
