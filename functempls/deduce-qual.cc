//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  template type deduction with qualifiers
//
//----------------------------------------------------------------------------

#include <string>

#include <boost/type_index.hpp>
#include <gtest/gtest.h>

#define CT_ERROR 0

template <typename T> std::string deduce(T x, T y) {
  return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}

template <typename T> std::string deduce2(const T &x, const T &y) {
  return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}

template <typename T> std::string deduce3(T &x, T &y) {
  return boost::typeindex::type_id_with_cvr<decltype(x)>().pretty_name();
}

TEST(functemplates, deduce_basic) {
  const int &a = 1;
  const int &b = 2;
  std::string s = deduce(a, b);
  EXPECT_EQ(s, "int");

  int x = 3;
  std::string s2 = deduce2(a, x);
  EXPECT_EQ(s2, "int");

  // we have deduction of const int& from T& (!)
  std::string s3 = deduce3(a, b);
  EXPECT_EQ(s3, "int const&");

#if CT_ERROR
  std::string s4 = deduce3(a, x);
#endif
}