//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Extended modules: demo
//
//-----------------------------------------------------------------------------

#include <iostream>

import first_extended;

int main() {
#ifdef NOPE
  std::cout << "foo: " << hello::foo() << std::endl;
#endif
  std::cout << "bar: " << hello::bar() << std::endl;
  std::cout << "e: " << hello::e << std::endl;
  std::cout << "buz: " << hello::buz() << std::endl;
}
