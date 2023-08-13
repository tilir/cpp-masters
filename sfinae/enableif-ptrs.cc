//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// enable if with pointers overload
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <type_traits>

namespace {

template <typename T, std::enable_if_t<(sizeof(T) > 4)> * = nullptr>
int foo(T x) {
  return 14;
}

template <typename T, std::enable_if_t<(sizeof(T) <= 4)> * = nullptr>
int foo(T x) {
  return 42;
}

} // namespace

TEST(sfinae, enableifptrs) {
  EXPECT_EQ(foo('c'), 42);
  EXPECT_EQ(foo(1.0), 14);
}