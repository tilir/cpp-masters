//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// fibonacci iterator example with check
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <numeric>
#include <ranges>
#include <vector>

namespace ranges = std::ranges;
namespace views = std::views;

class fib_iterator {
  int cur_ = 0, next_ = 1;

public:
  using value_type = int;
  using difference_type = int;

  int operator*() const { return cur_; }

  fib_iterator &operator++() {
    cur_ = std::exchange(next_, cur_ + next_);
    return *this;
  }

  fib_iterator operator++(int) {
    auto oldval = *this;
    ++*this;
    return oldval;
  }

  bool equals(const fib_iterator &rhs) const { return cur_ == rhs.cur_; }
};

static bool operator==(fib_iterator lhs, fib_iterator rhs) {
  return lhs.equals(rhs);
}
static bool operator!=(fib_iterator lhs, fib_iterator rhs) {
  return !(lhs == rhs);
}

static_assert(std::forward_iterator<fib_iterator>);

static int fib(int n) {
  int a = 0, b = 1, i;
  for (i = 0; i < n; ++i) {
    int tmp = b;
    b = a + b;
    a = tmp;
  }
  return a;
}

TEST(rngs, fibit) {
  fib_iterator fi;
  for (int i = 0; i < 10; ++i, ++fi) {
    EXPECT_EQ(*fi, fib(i));
  }
}