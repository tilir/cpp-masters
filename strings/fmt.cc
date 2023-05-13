//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of std::format
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

// format header works in GCC13+, --std=c++23
#include <format>
#include <iostream>
#include <sstream>
#include <string>

std::string foo(bool ssl, const char *path, const char *query) {
  std::stringstream ss;
  ss << (ssl ? "https" : "http") << "://" << path << "/" << query;
  auto s = ss.str();
  std::cout << s << std::endl;
  return s;
}

std::string bar(bool ssl, const char *path, const char *query) {
  auto fmt = std::format("{}://{}/{}", (ssl ? "https" : "http"), path, query);
  std::cout << fmt << std::endl;
  return fmt;
}

TEST(strings, hello) {
  auto S1 = foo(true, "mysite", "?q=1");
  EXPECT_EQ(S1, "https://mysite/?q=1");
  auto S2 = bar(false, "othersite", "?q=2");
  EXPECT_EQ(S2, "http://othersite/?q=2");
}

// crutch for clang on godbolt
#if defined(NOGTESTMAIN)
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif