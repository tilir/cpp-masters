//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Deduction guide for iterator pair
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

template <typename T> struct container {
  int n = 0;
  container(T t) : n(1) {}
  template <typename Iter> container(Iter beg, Iter end) : n(2) {}
};

template <typename Iter>
using value_t = typename std::iterator_traits<Iter>::value_type;

template <typename Iter> container(Iter b, Iter e) -> container<value_t<Iter>>;

TEST(classtempls, dedguide) {
  container c(7);                         // OK
  std::vector v = {1, 2, 3};              // OK (same reasons)
  auto d = container(v.begin(), v.end()); // OK: deduces T=double
  EXPECT_EQ(c.n, 1);
  EXPECT_EQ(d.n, 2);
}