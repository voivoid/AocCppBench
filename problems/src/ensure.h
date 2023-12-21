#pragma once

#include <cassert>
#include <stdexcept>

#if NDEBUG
#    define ensure(expr)                                                                                               \
        if (!(expr)) throw std::runtime_error(#expr);
#else
#    define ensure(expr) assert(expr)
#endif