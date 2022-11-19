//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Josuttis cases.
//
// Negative requires to semi-fix things.
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

#include <concepts>
#include <iostream>

struct MyString {
  MyString() {}
  MyString(const char *) { std::cout << "chr-copy" << std::endl; }
  MyString(const MyString &) { std::cout << "copy" << std::endl; }
  MyString(MyString &&) { std::cout << "move" << std::endl; }
  MyString &operator=(const MyString &) {
    std::cout << "copy" << std::endl;
    return *this;
  }
  MyString &operator=(MyString &&) {
    std::cout << "move" << std::endl;
    return *this;
  }
};

class Customer {
  MyString fst, snd;

public:
  template <typename S1, typename S2 = const char *>
  requires(!std::is_same_v<std::remove_reference_t<S1>, Customer>)
      Customer(S1 &&s1, S2 &&s2 = "")
      : fst(std::forward<S1>(s1)), snd(std::forward<S2>(s2)) {}
};

TEST(typeinf, jcase_negative) {
  std::cout << "--- 1. ---" << std::endl;
  Customer C("Johny", "Walker");
  std::cout << "--- 2. ---" << std::endl;
  MyString S = "Jack";
  Customer D(S, "Daniels");
  std::cout << "--- 3. ---" << std::endl;
  Customer E(std::move(S), "Daniels");
  std::cout << "--- 4. ---" << std::endl;
  Customer N("Nico");
  Customer M(N);
}