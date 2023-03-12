//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Virtual clone: CRTP mixin
//
//----------------------------------------------------------------------------

#include <iostream>
#include <memory>
#include <sstream>

#include "gtest/gtest.h"

namespace {

struct object_t {
  virtual void draw(std::ostream &, size_t pos) const = 0;
  virtual std::unique_ptr<object_t> clone() const = 0;
  virtual ~object_t() {}
};

template <typename Base, typename Derived> struct MixClonable : public Base {
  std::unique_ptr<Base> clone() const override {
    return std::unique_ptr<Base>(
        new Derived(static_cast<Derived const &>(*this)));
  }
};

struct int_t : public MixClonable<object_t, int_t> {
  int data;
  int_t(int data) : data(data) {}

  void draw(std::ostream &os, size_t pos) const override {
    os << std::string(pos, ' ') << data;
  }
};

} // namespace

TEST(rtpoly, virtual_clone_mixin) {
  std::ostringstream os;
  auto i = std::unique_ptr<object_t>(new int_t(42));
  auto j = i->clone();
  i->draw(os, 0);
  j->draw(os, 0);
  EXPECT_EQ(os.str(), "4242");
}