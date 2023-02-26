//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

#include <iostream>
#include <sstream>

#include "memuse.hpp"
#include "gtest/gtest.h"

namespace {

template <typename Derived> struct object_t {
  void draw(std::ostream &os, size_t pos) const {
    static_cast<const Derived *>(this)->draw(os, pos);
  }

protected:
  object_t() = default;
};

struct int_t : public object_t<int_t> {
  int data;
  int_t(int data) : data(data) {}
  void draw(std::ostream &os, size_t pos) const {
    os << std::string(pos, ' ') << data;
  }
};

template <typename T> void call_interface(object_t<T> &obj, std::ostream &os) {
  obj.draw(os, 0);
}

} // namespace

TEST(rtpoly, virtintf_crtp) {
  std::ostringstream os;
  int_t i = 42;
  call_interface(i, os);
  EXPECT_EQ(os.str(), "42");
}
