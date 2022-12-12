include(FetchContent)

set(BUILD_GMOCK OFF CACHE INTERNAL "")

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
  GIT_PROGRESS TRUE
  OVERRIDE_FIND_PACKAGE
)

find_package(googletest REQUIRED)