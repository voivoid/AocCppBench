#pragma once

#include <istream>
#include <optional>
#include <spanstream>
#include <string>
#include <vector>

namespace aoc
{

template <char delimiter = '\n'>
class lines
{
  public:
    friend std::istream& operator>>(std::istream& is, lines& l)
    {
        std::getline(is, l.m_line, delimiter);
        return is;
    }

    operator const std::string&() const
    {
        return m_line;
    }

  private:
    std::string m_line;
};

template <typename Element>
std::optional<size_t> peek_line_elements(std::istream& is)
{
    is.peek();  // force buffer availability

    const auto chars_avail = is.rdbuf()->in_avail();

    const auto current_pos = is.tellg();

    char* buff = reinterpret_cast<char*>(alloca(chars_avail + 1));
    is.get(buff, chars_avail + 1, '\0');

    is.seekg(current_pos);

    const auto* from = buff;
    const auto* to   = buff + chars_avail;
    auto iter        = std::find(from, to, '\n');
    if (iter == to) return {};

    std::ispanstream ss(std::span(from, iter));

    size_t elems = 0;
    Element elem;
    while (ss >> elem) { ++elems; }

    return { elems };
}

inline std::optional<size_t> peek_line_width(std::istream& is)
{
    return peek_line_elements<char>(is);
}

}  // namespace aoc