//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Addable concept: simplified but not working
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <type_traits>

template <typename T, typename... Ts>
constexpr inline bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

template <typename T, typename... Ts> struct first_arg { using type = T; };

template <typename... Ts> using first_arg_t = typename first_arg<Ts...>::type;

// clang-format off
template <typename... Ts> concept Addable = requires(Ts &&... args) {
  { (... + std::forward<Ts>(args)) } -> std::same_as<first_arg_t<Ts...>>;
  requires are_same_v<Ts...>;
  requires sizeof...(Ts) > 1;
};

template <typename T, typename... Ts>
concept Addable2 = requires(T &&arg, Ts &&... args) {
  { (std::forward<T>(arg) + ... + std::forward<Ts>(args)) } -> std::same_as<T>;
  requires are_same_v<Ts...>;
  requires sizeof...(Ts) > 0;
};
// clang-format on

template <typename... Ts> auto sum_all(Ts &&...args) requires Addable2<Ts...> {
  return (... + std::forward<Ts>(args));
}

TEST(variadic, addable2) {
  int res;
  res = sum_all(1, 2, 3, 4);
  EXPECT_EQ(res, 10);
}
