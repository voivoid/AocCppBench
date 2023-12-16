cmake_minimum_required(VERSION 3.28)

project(AocBenchCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/thirdparty/boost.cmake)
include(cmake/thirdparty/google_benchmark.cmake)
include(cmake/thirdparty/google_test.cmake)

if(MSVC)
    set(CMAKE_CXX_FLAGS "/EHsc /W4 /WX /MP /permissive- /Zc:preprocessor")
endif()

set(TEST_INPUT_DIR "${PROJECT_SOURCE_DIR}/tests/input")

add_subdirectory(problems)
add_subdirectory(solver)
add_subdirectory(bench)

enable_testing()
add_subdirectory(tests)

include(cmake/clang-format.cmake)