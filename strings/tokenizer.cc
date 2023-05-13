//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of boost::tokenizer
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

#include <boost/tokenizer.hpp>
#include <iostream>
#include <string>

using sep_t = boost::char_separator<char>;

TEST(strings, tokenizer) {
  std::string str = ";;Hello|world||-foo--bar;yow;baz|";
  std::ostringstream Os;

  sep_t sep("-;|");
  boost::tokenizer<sep_t> tokens(str, sep);
  for (auto tok : tokens)
    Os << "<" << tok << "> ";
  auto S = Os.str();
  std::cout << S << std::endl;

  EXPECT_EQ(S, "<Hello> <world> <foo> <bar> <yow> <baz> ");
}

// crutch for clang on godbolt
#if defined(NOGTESTMAIN)
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif