//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  template type deduction with complex types
//
//----------------------------------------------------------------------------

#include <string>

#include <boost/type_index.hpp>
#include <gtest/gtest.h>

namespace {

#define CT_ERROR 0

template <typename T> std::string deduce(T (*p)(T)) {
  return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}

int bar(int x) { return x; }

template <typename T, int N> std::string deduce2(T const (&)[N]) {
  return boost::typeindex::type_id_with_cvr<T>().pretty_name() + " " +
         std::to_string(N);
}

} // namespace

TEST(functemplates, deduce_extended) {
  std::string s = deduce(bar);
  EXPECT_EQ(s, "int");
}

TEST(functemplates, deduce_array) {
  std::string s = deduce2({1, 2, 3});
  EXPECT_EQ(s, "int 3");
}
