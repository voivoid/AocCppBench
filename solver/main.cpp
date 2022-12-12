#include "aoc/problems/all.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace
{
std::string get_input_file_name(const size_t year, const size_t day)
{
#ifndef AOC_TEST_INPUT_DIR
#    error AOC_TEST_INPUT_DIR is not defined
#endif

    std::stringstream ss;
    ss << AOC_TEST_INPUT_DIR << '/' << year << '/' << std::setfill('0') << std::setw(2) << day << ".txt";
    return ss.str();
}
}  // namespace

int main(int argc, char** argv)
{
    if (argc < 2 || argc > 4)
    {
        std::cerr << "Invalid arguments" << std::endl;
        return 1;
    }

    const auto* const problem_name = argv[ 1 ];

    const auto problem = aoc::problem_registrator::get_registered_problem(problem_name);
    if (!problem)
    {
        std::cerr << "Failed to find the problem " << problem_name << std::endl;
        return 1;
    }

    const std::string input_file = argc >= 3 ? argv[ 2 ] : get_input_file_name(problem->year, problem->day);

    std::ifstream input_fs(input_file, std::ios::in);
    if (!input_fs)
    {
        std::cerr << "Failed to open " << input_file << std::endl;
        return 1;
    }

    try
    {
        problem->solver(input_fs, std::cout);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Failed to solve the problem due to exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}