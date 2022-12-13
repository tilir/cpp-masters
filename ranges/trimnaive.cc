//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// trimming: naive approach
//
//----------------------------------------------------------------------------

#include <functional>
#include <numeric>
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

template <typename R> auto trim_front(R rng) {
  return rng | views::drop_while(::isspace);
}

template <typename R> auto trim_back(R rng) {
  return rng | views::reverse | views::drop_while(::isspace) | views::reverse;
}

template <typename R> auto trim(R rng) { return trim_back(trim_front(rng)); }

static std::string trim_str(const std::string &s) { return trim(s) | to_str(); }

TEST(rngs, trimnaive) {
  std::string s = "  \n\tHello  \n   ";
  auto t = trim_str(s);
  EXPECT_EQ(t, "Hello");
}