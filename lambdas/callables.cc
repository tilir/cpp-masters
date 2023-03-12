//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Different callables
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <type_traits>

namespace {

int foo() { return 42; }

using foo_t = std::decay_t<decltype(foo)>;

struct Sf {
  operator foo_t() { return foo; }
};

struct Sop {
  int operator()() { return 14; }
};

} // namespace

TEST(lamdas, callables) {
  foo_t pf = foo;
  EXPECT_EQ(pf(), 42);
  Sf sf;
  EXPECT_EQ(sf(), 42);
  Sop sop;
  EXPECT_EQ(sop(), 14);
}