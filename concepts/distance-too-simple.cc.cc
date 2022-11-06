//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Simple constraints have no partial ordering
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"

template <typename It>
struct is_input_iterator
    : std::is_base_of<std::input_iterator_tag,
                      typename std::iterator_traits<It>::iterator_category> {};

template <typename It>
struct is_random_iterator
    : std::is_base_of<std::random_access_iterator_tag,
                      typename std::iterator_traits<It>::iterator_category> {};

template <typename Iter>
requires is_input_iterator<Iter>::value int my_distance(Iter first, Iter last) {
  int n = 0;
  while (first != last) {
    ++first;
    ++n;
  }
  return n;
}

template <typename Iter>
requires is_random_iterator<Iter>::value int my_distance(Iter first,
                                                         Iter last) {
  return last - first;
}

TEST(concepts, overload) {
  std::vector<int> v = {2, 3, 1, 4, 5};
  auto dist = my_distance(v.begin(), v.end()); // fail
  EXPECT_EQ(dist, 5);
}