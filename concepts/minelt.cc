//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// min_element example
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>
#include <forward_list>
#include <vector>

template <typename T, typename = void>
struct is_totally_ordered : std::false_type {};

template <typename T>
struct is_totally_ordered<
    T, std::void_t<decltype(std::declval<T>() == std::declval<T>()),
                   decltype(std::declval<T>() < std::declval<T>())>>
    : std::true_type {};

template <typename It>
struct is_forward_iterator
    : std::is_base_of<std::forward_iterator_tag,
                      typename std::iterator_traits<It>::iterator_category> {};

template <typename Iter>
#if !defined(NOCONSTR)
requires is_forward_iterator<Iter>::value &&
    is_totally_ordered<typename Iter::value_type>::value
#endif
        Iter
        my_min_element(Iter first, Iter last) {
  Iter min = first;
  while (first != last) {
    if (*first < *min)
      min = first;
    ++first;
  }
  return min;
}

TEST(concepts, combine) {
#ifdef WRONG1
  struct S1 {
    bool operator==(struct S1) { return false; }
    bool operator<(struct S1) { return false; }

    struct Iter {
      using value_type = S1;
    };

    Iter begin() { return Iter{}; }
    Iter end() { return Iter{}; }
  };

  S1 s1;
  auto minw1 = my_min_element(s1.begin(), s1.end());
#endif

#ifdef WRONG2
  struct S2 {};

  std::forward_list<S2> w;
  auto minw2 = my_min_element(w.begin(), w.end());
#endif

  std::forward_list<int> f = {2, 3, 1, 4, 5};
  auto minf = my_min_element(f.begin(), f.end()); // ok

  std::vector<int> v = {2, 3, 1, 4, 5};
  auto minv = my_min_element(v.begin(), v.end()); // ok

  EXPECT_EQ(*minf, *minv);
}