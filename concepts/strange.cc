//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Lazy evaluation
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

template <typename T>
concept Strange = (sizeof(T) == 4) || (requires() {
                    { T::value }
                    ->std::convertible_to<bool>;
                  } && (T::value == true));

template <typename T> requires Strange<T> int f(T t) { return 42; }

TEST(concepts, strange) {
  int x = f(1);
  EXPECT_EQ(x, 42);
}