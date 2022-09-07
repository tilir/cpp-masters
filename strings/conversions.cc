//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of lvalue to rvalue conversions
//
//----------------------------------------------------------------------------

#include <cstddef>
#include <iostream>

int foo() {
  volatile int a = 10;
  int b = a;
  return b;
}

int *bar() {
  volatile std::nullptr_t a = nullptr;
  int *b;
  b = a;
  return b;
}