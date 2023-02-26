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

struct object_t {
  virtual void draw(std::ostream &, size_t pos) const = 0;
  virtual ~object_t() {}
};

struct int_t : public object_t {
  int data;
  int_t(int data) : data(data) {}
  void draw(std::ostream &os, size_t pos) const override {
    os << std::string(pos, ' ') << data;
  }
};

void call_interface(object_t &obj, std::ostream &os) { obj.draw(os, 0); }

} // namespace

TEST(rtpoly, virtintf) {
  std::ostringstream os;
  int_t i = 42;
  call_interface(i, os);
  EXPECT_EQ(os.str(), "42");
}
