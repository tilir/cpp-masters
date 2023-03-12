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

#include <boost/tokenizer.hpp>
#include <iostream>
#include <string>

using sep_t = boost::char_separator<char>;

int main() {
  std::string str = ";;Hello|world||-foo--bar;yow;baz|";

  sep_t sep("-;|");
  boost::tokenizer<sep_t> tokens(str, sep);
  for (auto tok : tokens)
    std::cout << "<" << tok << "> ";
  std::cout << std::endl;
}