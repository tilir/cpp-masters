#include "gtest/gtest.h"
#include <concepts>

namespace {

template <typename T, typename U> int foo(T, U) { return 1; }

template <typename T, typename U> int foo(T *, U *) { return 2; }

template <> int foo<int *, int *>(int *, int *) { return 3; }

} // namespace

TEST(functemplates, crocodile) {
  int x;
  int res = foo(&x, &x);
  EXPECT_EQ(res, 2);
}