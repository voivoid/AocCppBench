include(FetchContent)

set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")

FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark
  GIT_TAG v1.8.3
  GIT_PROGRESS TRUE
  OVERRIDE_FIND_PACKAGE
)

find_package(googlebenchmark REQUIRED)