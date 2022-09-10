//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Working with non type parameters.
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

struct Pair {
  int x = 1, y = 1;
};

template <int N, int *PN, int &RN, Pair P> int foo() {
  return N + *PN + RN + P.x + P.y;
}

constexpr Pair p;
int x = 1;
int y = 1;

TEST(nontypes, foo) {
  int res = foo<2, &x, y, p>();
  EXPECT_EQ(res, 2 + 1 + 1 + 1 + 1);
}

#if 0

class PairEnc {
  int x = 1, y = 1;
};

template <int N, int *PN, int &RN, PairEnc P> int bar() {
  return N + *PN + RN + P.x + P.y;
}

constexpr PairEnc pe;

TEST(nontypes, bar) {
  int res = bar<2, &x, y, pe>();
  EXPECT_EQ(res, 2 + 1 + 1 + 1 + 1);
}

#endif

struct MoreComplex : public Pair {
  int arr[3] = {1, 2, 3};
  int z = 1;
};

template <int N, int *PN, int &RN, MoreComplex M> int buz() {
  return N + *PN + RN + M.z + M.arr[0];
}

constexpr MoreComplex mc;

TEST(nontypes, buz) {
  int res = buz<2, &x, y, mc>();
  EXPECT_EQ(res, 2 + 1 + 1 + 1 + 1);
}
