//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Compile time map
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <array>
#include <string>

template <typename P, size_t N>
auto constexpr access(P const (&a)[N], typename P::first_type k) {
  size_t i = 0;
  for (; i < N; ++i)
    if (k == a[i].first)
      return a[i].second;
  if (i == N)
    throw "not found";

  // silence compiler: we will never be here
  __builtin_unreachable();
}

template <typename ElemT, size_t N> struct ct_map_t {
  using KeyT = typename ElemT::first_type;
  using ValueT = typename ElemT::second_type;

  ElemT arr_[N];

  template <typename T, size_t... Is>
  constexpr ct_map_t(T (&data)[N], std::index_sequence<Is...>)
      : arr_{{data[Is].first, data[Is].second}...} {}

  template <typename T>
  constexpr ct_map_t(T (&arr)[N])
      : ct_map_t(arr, std::make_index_sequence<N>{}) {}

  constexpr ValueT operator[](KeyT k) const {
    return access<ElemT, N>(arr_, k);
  }
};

template <typename Key, typename Value, size_t N>
constexpr auto
make_ct_map(const std::pair<const Key, const Value> (&items)[N]) {
  return ct_map_t<std::pair<Key, Value>, N>(items);
}

TEST(cexpr, ctmap) {
  constexpr auto ct_map = make_ct_map<std::string_view, int>(
      {{"one", 1}, {"two", 2}, {"three", 3}});
  constexpr int result = ct_map["three"];
  EXPECT_EQ(result, 3);
}