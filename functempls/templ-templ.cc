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

namespace {

template <template <typename> typename Container, typename Element>
int get_size(const Container<Element> &A) {
  return A.size();
}

template <typename T> struct MyCont {
  int size() const { return 0; }
};

// no size function
template <typename T> struct OtherCont {};

template <> int get_size<OtherCont, int>(const OtherCont<int> &A) { return 10; }

} // namespace

TEST(functemplates, templtempl) {
  MyCont<int> Mc;
  int Sz = get_size<MyCont, int>(Mc);
  EXPECT_EQ(Sz, 0);

  OtherCont<int> Oc;
  Sz = get_size<OtherCont, int>(Oc);
  EXPECT_EQ(Sz, 10);
}