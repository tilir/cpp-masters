//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// ODR usage example (with slight clang/gcc disagreement)
//
//-----------------------------------------------------------------------------

#include <iostream>

extern int x;

int foo() {
  if constexpr (false) {
    return x;
  } else {
    return 0;
  }
}

template <int N> int bar() {
  if constexpr (false) {
    return x;
  } else {
    return N;
  }
}

int main() { return foo() + bar<0>(); }