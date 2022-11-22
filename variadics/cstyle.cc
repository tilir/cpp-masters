//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// C-style variadic packs
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <cstdarg>

int sum_all(int nargs, ...) {
  va_list ap;
  int cnt = 0;
  va_start(ap, nargs);

  for (int i = 0; i < nargs; ++i)
    cnt += va_arg(ap, int);

  va_end(ap);
  return cnt;
}

#define NUMARGS(...) (sizeof((int[]){__VA_ARGS__}) / sizeof(int))
#define DOSUM(...) sum_all(NUMARGS(__VA_ARGS__), __VA_ARGS__)

TEST(variadic, cstyle) {
  int res = DOSUM(1, 2, 3, 4);
  EXPECT_EQ(res, 10);
}