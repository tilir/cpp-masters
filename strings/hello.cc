//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of string literals
//
//----------------------------------------------------------------------------

#include <iostream>

int main() {
  const char *cc = "Hello, world!";
  constexpr char cp[14] = "Hello, world!";
  std::cout << sizeof("Hello, world!") << " : "
            << "Hello, world!" << std::endl;
  std::cout << sizeof(cc) << " : " << cc << std::endl;
  std::cout << sizeof(cp) << " : " << cp << std::endl;

  char c = "Hello"[1];
  std::cout << c << std::endl;

  std::cout << static_cast<const void *>("Hello world") << std::endl;
  std::cout << static_cast<const void *>(cc) << std::endl;
  std::cout << static_cast<const void *>(cp) << std::endl;

  const char *vertical =
      R"(h
e
l
l
o)";

  std::cout << vertical << std::endl;
}
