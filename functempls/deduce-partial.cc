//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  template type deduction with partial specification
//
//----------------------------------------------------------------------------

#include <string>

#include <boost/type_index.hpp>
#include <gtest/gtest.h>

#define CT_ERROR 0

template <typename DstT, typename SrcT> DstT implicit_cast(SrcT x) { return x; }

TEST(deduce, context) {
#if CT_ERROR
  double v0 = implicit_cast(-1); // fail!
#endif
  double v1 = implicit_cast<double, int>(-1); // ok
  double v2 = implicit_cast<double>(-1);      // ok
  EXPECT_EQ(v1, -1.0);
  EXPECT_EQ(v2, -1.0);
}
