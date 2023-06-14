//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  template type deduction with NTTP
//
//----------------------------------------------------------------------------

#include <boost/type_index.hpp>
#include <gtest/gtest.h>

#define CT_ERROR 0

namespace {

template <auto n> int foo() { return n; }

} // namespace

TEST(functemplates, context) {
  double v0 = foo<1>();
  EXPECT_EQ(v0, 1);

// still not supported in CLANG 16
#if 0
  double v1 = foo<2.0>();
  EXPECT_EQ(v1, 2);
#endif
}