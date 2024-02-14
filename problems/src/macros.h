#pragma once

#ifdef _MSC_VER
#    define AOC_MSVC_DISABLE_WARNING(warning_id) __pragma(warning(disable : warning_id))
#    define AOC_MSVC_DISABLE_WARNINGS_PUSH(warning_id) __pragma(warning(push)) AOC_MSVC_DISABLE_WARNING(warning_id)
#    define AOC_MSVC_DISABLE_WARNINGS_POP() __pragma(warning(pop))
#else
#    define AOC_MSVC_DISABLE_WARNING(warning_id)
#    define AOC_MSVC_DISABLE_WARNINGS_PUSH(warning_id)
#    define AOC_MSVC_DISABLE_WARNINGS_POP()
#endif
