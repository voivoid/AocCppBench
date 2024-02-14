cmake_minimum_required(VERSION 3.28)

project(AocBenchCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(AOC_COMPILE_DEFS /EHsc /MP /permissive- /volatile:iso /Zc:preprocessor,inline,lambda,__cplusplus,externConstexpr,throwingNew,templateScope,enumTypes)
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:${AOC_COMPILE_DEFS}>")
endif()

include(cmake/thirdparty/boost.cmake)
include(cmake/thirdparty/google_benchmark.cmake)
include(cmake/thirdparty/google_test.cmake)

set(TEST_INPUT_DIR "${PROJECT_SOURCE_DIR}/tests/input")

if(MSVC)
    set(AOC_WARNING_OPTS /W4 /WX)
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:${AOC_WARNING_OPTS}>")
endif()

add_subdirectory(problems)
add_subdirectory(solver)
add_subdirectory(bench)

enable_testing()
add_subdirectory(tests)

include(cmake/clang-format.cmake)