//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Prvalue elision sample
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory_resource>

#include "gtest/gtest.h"

namespace {

template <typename T> class ScopedPointer {
  T *value;

public:
  ScopedPointer(T *value) : value(value) {}
  ScopedPointer(const ScopedPointer &rhs) = delete;
  ScopedPointer &operator=(const ScopedPointer &rhs) = delete;
  ScopedPointer(ScopedPointer &&rhs) = delete;
  ScopedPointer &operator=(ScopedPointer &&rhs) = delete;

  T operator*() const { return *value; }
};

int foo(ScopedPointer<int> s) { return *s; }

auto bar() {
  return ScopedPointer<int>{new int(42)}; // OK
}

#if defined(BUG)
auto buz() {
  ScopedPointer<int> t{new int(42)};
  return t; // FAIL, NRVO case
}
#endif

} // namespace

TEST(smartptrs, prvalue) {
  int n = foo(ScopedPointer<int>{new int(42)}); // OK
  EXPECT_EQ(n, 42);
  auto k = bar(); // OK
  EXPECT_EQ(*k, 42);
}
