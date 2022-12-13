//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// range with unreachable sentinel
//
//----------------------------------------------------------------------------

#include "boost/type_index.hpp"
#include "gtest/gtest.h"
#include <ranges>
#include <vector>

namespace ti = boost::typeindex;
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

constexpr int N = 10;

TEST(rngs, unreachable) {
  fib_iterator fi;
  auto uv = ranges::subrange{fi, std::unreachable_sentinel};
  std::vector<int> v(N);

  std::cout << ti::type_id_with_cvr<decltype(uv)>().pretty_name() << std::endl;
  ranges::copy_n(uv.begin(), N, v.begin());

  for (int i = 0; i < N; ++i) {
    EXPECT_EQ(v[i], fib(i));
  }
  auto cuv = std::ranges::common_view{uv};
  // OK for CUV not for UV
  std::copy_n(cuv.begin(), N, v.begin());
}