//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  generate specialization then delete it
//  failure is "redefinition"
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

template <typename T> T max(T x, T y) { return x > y ? x : y; }

template <> int max<int>(int x, int y) { return x > y ? x : y; }

template <> int max<int>(int x, int y) = delete;

TEST(max_test_case, testWillPass) { EXPECT_EQ(max(2, 11), 11); }