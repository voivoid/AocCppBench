#pragma once

#include <spanstream>
#include <string_view>

namespace
{
template <typename Ret, typename... Args>
Ret solve(const char* const input, Ret (*f)(std::istream&, Args...), Args... args)
{
    std::ispanstream ss(std::span(input, strlen(input)));
    return f(ss, std::forward<Args>(args)...);
}

}  // namespace
