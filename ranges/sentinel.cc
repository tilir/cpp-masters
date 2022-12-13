//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// std::default_sentinel use case
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <numeric>
#include <ranges>
#include <vector>

namespace ranges = std::ranges;

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

bool operator==(fib_iterator lhs, fib_iterator rhs) { return lhs.equals(rhs); }
bool operator!=(fib_iterator lhs, fib_iterator rhs) { return !(lhs == rhs); }

static_assert(std::forward_iterator<fib_iterator>);

int fib(int n) {
  int a = 0, b = 1, i;
  for (i = 0; i < n; ++i) {
    int tmp = b;
    b = a + b;
    a = tmp;
  }
  return a;
}

constexpr int N = 10;

TEST(rngs, sentinel) {
  fib_iterator fi;
  auto ci = std::counted_iterator{fi, N};
  auto sv = ranges::subrange{ci, std::default_sentinel};
  std::vector<int> v(N);
  ranges::copy(sv, v.begin());

#if 0
  std::copy(sv.begin(), sv.end(), v.begin());
#endif

  for (int i = 0; i < N; ++i) {
    EXPECT_EQ(v[i], fib(i));
  }
}