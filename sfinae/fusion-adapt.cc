//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// boost fusion example
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <array>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/container.hpp>
#include <boost/fusion/functional.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/support.hpp>
#include <boost/fusion/tuple.hpp>
#include <cmath>
#include <concepts>
#include <string>
#include <tuple>
#include <type_traits>

namespace fusion = boost::fusion;

struct pythagoras {
  typedef double result_type;
  template <typename T> double operator()(double acc, T const &axis) const {
    double d = fusion::at_c<0>(axis) - fusion::at_c<1>(axis);
    return acc + d * d;
  }
};

template <typename P1, typename P2> double pydist(P1 p1, P2 p2) {
  typedef fusion::vector<P1 &, P2 &> zip_t;
  double accumulated =
      fusion::fold(fusion::zip_view<zip_t>(zip_t(p1, p2)), 0, pythagoras());
  return sqrt(accumulated);
}

struct mypoint {
  double x = 0.0, y = 0.0;
  mypoint() = default;
  mypoint(double x, double y) : x(x), y(y) {}
};

BOOST_FUSION_ADAPT_STRUCT(mypoint, (double, x)(double, y));

TEST(sfinae, fusion) {
  mypoint a{2.0, 0.0};
  mypoint b{4.0, 0.0};
  double d2 = pydist(a, b);
  EXPECT_EQ(d2, 2.0);
}