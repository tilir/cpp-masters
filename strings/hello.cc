//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Some examples for string literals
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

#include <sstream>

const char *answer =
    R"(14 : Hello, world!
8 : Hello, world!
14 : Hello, world!
e
h
e
l
l
o)";

TEST(strings, hello) {
  const char *cc = "Hello, world!";
  constexpr char cp[14] = "Hello, world!";
  std::ostringstream Os;

  Os << sizeof("Hello, world!") << " : "
     << "Hello, world!" << std::endl;
  Os << sizeof(cc) << " : " << cc << std::endl;
  Os << sizeof(cp) << " : " << cp << std::endl;

  char c = "Hello"[1];
  Os << c << std::endl;

#if defined(VISUALIZE)
  std::cout << static_cast<const void *>("Hello world") << std::endl;
  std::cout << static_cast<const void *>(cc) << std::endl;
  std::cout << static_cast<const void *>(cp) << std::endl;
#endif

  const char *vertical =
      R"(h
e
l
l
o)";

  Os << vertical;
  std::string Ans = answer;
  EXPECT_EQ(Ans, Os.str());
}

// crutch for clang on godbolt
#if defined(NOGTESTMAIN)
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif