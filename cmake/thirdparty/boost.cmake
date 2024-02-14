include(FetchContent)

set(BOOST_ENABLE_CMAKE ON)
set(BOOST_INCLUDE_LIBRARIES static_string circular_buffer hana icl container fusion variant unordered geometry context uuid)
FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
  URL_MD5 893b5203b862eb9bbd08553e24ff146a
  USES_TERMINAL_DOWNLOAD TRUE
)
FetchContent_MakeAvailable(Boost)