//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  template template parameters
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

template <template <typename> typename Container, typename Element>
int get_size(const Container<Element> &a) {
  return a.size();
}

template <typename T> struct MyCont {
  int size() const { return 0; }
};

// no size function
template <typename T> struct OtherCont {};

template <> int get_size<OtherCont, int>(const OtherCont<int> &a) { return 10; }

TEST(templtempl, basic) {
  MyCont<int> c;
  int sz = get_size<MyCont, int>(c);
  EXPECT_EQ(sz, 0);
}

TEST(templtempl, spec) {
  OtherCont<int> c;
  int sz = get_size<OtherCont, int>(c);
  EXPECT_EQ(sz, 10);
}