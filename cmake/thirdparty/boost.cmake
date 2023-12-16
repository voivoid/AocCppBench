include(ExternalProject)

set(BoostVersion "1.84.0")
set(BoostSHA256 a5800f405508f5df8114558ca9855d2640a2de8f0445f051fa1c7c3383045724)

string(REPLACE . _ BoostVersionUnderscored ${BoostVersion})
ExternalProject_Add(
  get_boost
  URL "https://boostorg.jfrog.io/artifactory/main/release/${BoostVersion}/source/boost_${BoostVersionUnderscored}.tar.gz"
  URL_HASH SHA256=${BoostSHA256}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  BUILD_IN_SOURCE TRUE
  INSTALL_COMMAND ""
  BUILD_BYPRODUCTS
)

ExternalProject_Get_Property(get_boost SOURCE_DIR)
set(BoostSrcDir ${SOURCE_DIR})
set(BoostLibDir ${BoostSrcDir}/stage/lib)

add_library(boost_headers INTERFACE)
add_dependencies(boost_headers get_boost)
add_library(Boost::boost ALIAS boost_headers)

set_target_properties(boost_headers PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${BoostSrcDir})
set_target_properties(boost_headers PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${BoostSrcDir})

if(MSVC)
  set_property(TARGET boost_headers PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_ALL_NO_LIB)
endif()