//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Movable entities sample
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory_resource>

#include "gtest/gtest.h"

namespace {

template <typename T> class UniquePointer {
  T *value;

public:
  UniquePointer(T *value) : value(value) {}
  UniquePointer(const UniquePointer &rhs) = delete;
  UniquePointer &operator=(const UniquePointer &rhs) = delete;
  UniquePointer(UniquePointer &&rhs) : value(rhs.value) { rhs.value = nullptr; }
  UniquePointer &operator=(UniquePointer &&rhs) {
    std::swap(value, rhs.value);
    return *this;
  }

  T operator*() const { return *value; }
};

decltype(auto) foo() {
  UniquePointer<int> t{new int(42)};
  return t;
}

#if defined(BUG)
decltype(auto) bar() {
  UniquePointer<int> t{new int(42)};
  return std::move(t);
}
#endif

} // namespace

TEST(smartptrs, movable) {
  auto n = foo();
  EXPECT_EQ(*n, 42); // OK

#if defined(BUG)
  auto k = bar();
  EXPECT_EQ(*k, 42); // FAIL, dangling ref
#endif
}
