#include "input.h"

#include <cassert>
#include <iostream>
#include <string>


namespace aoc
{

void skip_eol(std::istream& input)
{
    if (input.peek() == '\r')
        input.get();
    if (input.peek() == '\n')
        input.get();
}

}  // namespace aoc