//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Distance not working (use --std=c++17 or earlier) but why?
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <iterator>
#include <type_traits>

namespace {

class junk_iter_t {
  int *state_;

public:
  using iterator_type = junk_iter_t;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = int;
  using difference_type = int;

public:
  junk_iter_t(int *state = nullptr) : state_(state) {}
  junk_iter_t(const junk_iter_t &) = default;
  junk_iter_t(junk_iter_t &&) = default;
  junk_iter_t &operator=(const junk_iter_t &) = default;
  junk_iter_t &operator=(junk_iter_t &&) = default;

public:
  junk_iter_t &operator++() {
    ++state_;
    return *this;
  }

  junk_iter_t operator++(int) {
    auto temp(*this);
    ++(*this);
    return temp;
  }

  junk_iter_t &operator+=(int n) {
    for (int i = 0; i < n; ++i)
      ++(*this);
    return *this;
  }

  junk_iter_t &operator--() {
    --state_;
    return *this;
  }

  junk_iter_t operator--(int) {
    auto temp(*this);
    --(*this);
    return temp;
  }

  junk_iter_t &operator-=(int n) {
    for (int i = 0; i < n; ++i)
      --(*this);
    return *this;
  }

  value_type &operator*() const { return *state_; }
  value_type *operator->() const { return state_; }

  bool equals(const junk_iter_t &rhs) const { return state_ == rhs.state_; }
};

static inline bool operator==(junk_iter_t lhs, junk_iter_t rhs) {
  return lhs.equals(rhs);
}

static inline bool operator!=(junk_iter_t lhs, junk_iter_t rhs) {
  return !lhs.equals(rhs);
}

static inline junk_iter_t operator+(junk_iter_t it, int n) {
  it += n;
  return it;
}

static inline junk_iter_t operator-(junk_iter_t it, int n) {
  it -= n;
  return it;
}

} // namespace

TEST(concepts, distancemystery) {
  int arr[10];
  junk_iter_t fst(arr), snd(arr + 3);
  auto dist = std::distance(fst, snd);

  EXPECT_EQ(dist, 3);
}