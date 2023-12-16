#pragma once

#include <spanstream>
#include <string_view>

namespace
{
template <typename Ret>
Ret solve(const char* const input, Ret (*f)(std::istream&))
{
    std::ispanstream ss(std::span(input, strlen(input)));
    return f(ss);
}

}  // namespace
