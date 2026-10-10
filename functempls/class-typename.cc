//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  class vs typename holywar solved with concepts
//
//----------------------------------------------------------------------------

#include <concepts>
#include <string>

#include "gtest/gtest.h"

namespace {

#if 0
template <typename T> int foo(T x) { return x; }
template <class T> int foo(T x) { return x + 1; }
#endif

template <std::integral T>
int foo(T x) {
  return x % 2;
}

} // namespace

TEST(functemplates, class_typename) {
  int res = foo(11);
  EXPECT_EQ(res, 1);
}

#if 0
TEST(functemplates, class_typename_err) {
  EXPECT_EQ(foo(0.0), 2);
}
#endif