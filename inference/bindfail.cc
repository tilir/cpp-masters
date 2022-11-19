//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Binding failures
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

TEST(rvrefs, binding_failures) {
  int x = 1;

  int &&y = x * 3; // ok

#if FAIL
  int &&b = x; // fail, не rvalue
#endif

#if FAIL
  int &c = x * 3; // fail, не lvalue
#endif

  const int &d = x * 3; // ok, продляет время жизни

#if FAIL
  int &&e = y; // fail, не rvalue
#endif

  int &f = y; // ok
  EXPECT_EQ(f, 3);

  const int &g = y; // ok
  f += 1;
  EXPECT_EQ(g, 4);
}