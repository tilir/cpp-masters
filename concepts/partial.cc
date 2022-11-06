//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Partial ordering with specialization.
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

template <typename T> concept Ord = requires(T a, T b) { a < b; };
template <typename T> concept Inc = requires(T a) { ++a; };
template <typename T> concept Void = std::is_same_v<T, void>;

template <typename T> requires Ord<T> || Inc<T> || Void<T> struct less {
  int operator()() const { return 2; }
};

template <Ord T> struct less<T> {
  int operator()() const { return 22; }
};

template <> struct less<void> {
  int operator()() const { return 42; }
};

struct NoComp {
  void operator++() {}
};

TEST(concepts, ordvoid) {
  less<void> l1;
  less<double> l2;
  less<NoComp> l3;

  EXPECT_EQ(l1(), 42);
  EXPECT_EQ(l2(), 22);
  EXPECT_EQ(l3(), 2);
}