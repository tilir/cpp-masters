//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Transparent function: not really but close
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <memory>

namespace {

template <typename Fun, typename... Args>
decltype(auto) transparent(Fun fun, Args &&... args) {
  return fun(std::forward<Args>(args)...);
}

} // namespace

TEST(lamdas, transparent) {
  auto a = std::make_unique<int>(1);
  auto lmd = [uptr = std::move(a)](int x) { return *uptr + x; };
  auto result = transparent(lmd, 3); // oops
  EXPECT_EQ(result, 4);
}