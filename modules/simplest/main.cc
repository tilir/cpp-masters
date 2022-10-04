//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// First simple module usage
//
//-----------------------------------------------------------------------------

#include <iostream>

import first_module;

int main() {
#ifdef NOPE
  std::cout << "foo: " << hello::foo() << std::endl;
#endif
  std::cout << "bar: " << hello::bar() << std::endl;
  std::cout << "e: " << hello::e << std::endl;
}