//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// to string: simple adaptor
//
//----------------------------------------------------------------------------

#include <ranges>
#include <string>
#include <vector>

#include "gtest/gtest.h"

namespace views = std::views;

template <typename R> std::string to_string(R rng) {
  auto v = views::common(rng);
  return std::string(v.begin(), v.end());
}

struct to_str_closure {
  template <typename R> auto operator()(R r) const { return to_string(r); }
};

struct to_str_adaptor {
  template <typename R> auto operator()(R r) { return to_string(r); }

  auto operator()() { return to_str_closure(); }
};

template <typename R> auto operator|(R r, const to_str_closure &c) {
  return c(r);
}

inline to_str_adaptor to_str;

TEST(rngs, tostradaptor) {
  std::vector v = {'a', 'b', 'c', 'd'};
  auto s = v | views::filter([](int n) { return n % 2; }) | to_str();
  EXPECT_EQ(s, "ac");
}