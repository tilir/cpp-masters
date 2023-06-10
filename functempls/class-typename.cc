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

#include "gtest/gtest.h"
#include <concepts>

#if 0
template <typename T> int foo(T x) { return x; }
template <class T> int foo(T x) { return x + 1; }
#endif

template <std::integral T> int foo(T x) { return x % 2; }

TEST(functemplates, class_typename) { EXPECT_EQ(foo(11), 1); }

#if 0
TEST(functemplates, class_typename_err) {
  EXPECT_EQ(foo(0.0), 2);
}
#endif