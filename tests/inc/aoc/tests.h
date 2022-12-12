#pragma once

#include <sstream>

namespace
{
template <typename Ret>
Ret solve( const char* const input, Ret ( *f )( std::istream& ) )
{
  std::istringstream ss( input );
  return f( ss );
}

}  // namespace