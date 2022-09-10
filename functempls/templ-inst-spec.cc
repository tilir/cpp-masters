//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  template instantiation vs specialization
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

#define EARLY 0
#define SPECIALIZE 1

template <typename T> T max(T x, T y) { return x > y ? x : y; }

#if (EARLY == 1)
extern template int max<int>(int, int);
#endif

#if (SPECIALIZE == 1)
template <> int max<int>(int x, int y) { return x > y ? x : y; }
#endif

#if (EARLY == 0)
extern template int max<int>(int, int);
#endif

TEST(maxmin, maxorder) { EXPECT_EQ(max(2, 11), 11); }