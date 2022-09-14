//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Demo of instantiations, main function
//
//----------------------------------------------------------------------------

#include <iostream>

#include "max.hpp"

int main() {
  int foores = foo(0, 0);
  int barres = bar(0, 0);
  std::cout << "foo: " << foores << "; bar: " << barres << std::endl;
  return 0;
}
