//-----------------------------------------------------------------------------
//
// Source code for MIPT course on informatics
// Page with slides: http://cs.mipt.ru/wp/?page_id=7775
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
  }
  else {
    return 0;
  }
}

template <int N> int bar() {
  if constexpr (false) {
    return x;
  }
  else {
    return N;
  }
}


int main() {
  return foo() + bar<0>();
}