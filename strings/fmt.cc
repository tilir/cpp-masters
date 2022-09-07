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

#include <iostream>
#include <format>
#include <sstream>
#include <string>

void foo(bool ssl, const char *path, const char *query) {
  std::stringstream ss;
  ss << (ssl ? "https" : "http") << "://" << path << "/" << query;
  std::cout << ss.str() << std::endl;
}

void bar(bool ssl, const char *path, const char *query) {
  auto fmt = std::format("{}://{}/{}", (ssl ? "https" : "http"), path, query);
  std::cout << fmt << std::endl;
}

int main() {
  foo(true, "mysite", "?q=1");
  bar(false, "othersite", "?q=2");
}