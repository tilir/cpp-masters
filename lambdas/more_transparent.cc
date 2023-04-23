//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Transparent function: reinventing std invoke
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <memory>

namespace {

template <typename Fun, typename... Args>
decltype(auto) transparent(Fun &&fun, Args &&...args) {
  return std::forward<Fun>(fun)(std::forward<Args>(args)...);
}

} // namespace

TEST(lamdas, more_transparent) {
  auto a = std::make_unique<int>(1);
  auto lmd = [uptr = std::move(a)](int x) { return *uptr + x; };
  auto result = transparent(lmd, 3); // ok
  EXPECT_EQ(result, 4);
}