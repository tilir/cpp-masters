//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// naive not working enable if overload
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <type_traits>

namespace {

template <typename T, typename = std::enable_if_t<(sizeof(T) > 4)>>
int foo(T x) {
  return 14;
}

template <typename T, typename = std::enable_if_t<(sizeof(T) <= 4)>>
int foo(T x) {
  return 42;
}

} // namespace

TEST(sfinae, naiveovr) {
  EXPECT_EQ(foo('c'), 14);
  EXPECT_EQ(foo(1.0), 42);
}