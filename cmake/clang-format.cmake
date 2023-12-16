find_program(ClangFormat clang-format)
if(ClangFormat)
  set(AocTargets aoc_problems aoc_tests aoc_solver aoc_bench)
  foreach(AocTarget ${AocTargets})
    get_target_property(AocTargetSources ${AocTarget} SOURCES)
    get_target_property(AocTargetDir ${AocTarget} SOURCE_DIR)
    foreach(Src ${AocTargetSources})
      list(APPEND AocSources "${AocTargetDir}/${Src}")
    endforeach()
  endforeach()

  list(FILTER AocSources INCLUDE REGEX "^.*\.(h|cpp)\$")

  add_custom_target(clangformat
    COMMAND clang-format -style=file -i ${AocSources}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()