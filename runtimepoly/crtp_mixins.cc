//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// CRTP mixins -- classical comparisons demo
//
//----------------------------------------------------------------------------

#include <iostream>
#include <sstream>

#include "gtest/gtest.h"

namespace {

template <typename Derived> struct Comparisons {};

template <typename Derived>
bool operator==(const Comparisons<Derived> &o1,
                const Comparisons<Derived> &o2) {
  const Derived &d1 = static_cast<const Derived &>(o1);
  const Derived &d2 = static_cast<const Derived &>(o2);
  return !(d1 < d2) && !(d2 < d1);
}

template <typename Derived>
bool operator!=(const Comparisons<Derived> &o1,
                const Comparisons<Derived> &o2) {
  return !(o1 == o2);
}

template <typename Derived> struct object_t {
  void draw(std::ostream &os, size_t pos) const {
    static_cast<const Derived *>(this)->draw(os, pos);
  }

protected:
  object_t() = default;
};

struct int_t : public object_t<int_t>, public Comparisons<int_t> {
  int data;
  int_t(int data) : data(data) {}
  void draw(std::ostream &os, size_t pos) const {
    os << std::string(pos, ' ') << data;
  }
  bool less(const int_t &rhs) const { return data < rhs.data; }
};

bool operator<(const int_t &lhs, const int_t &rhs) { return lhs.less(rhs); }
} // namespace

TEST(rtpoly, crtp_mixins) {
  std::ostringstream os;
  int_t i = 42;
  int_t j = 42;
  EXPECT_EQ(i == j, true);
}
