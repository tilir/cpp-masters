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
// Positive solution.
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
  requires(std::is_convertible_v<S1, MyString>) Customer(S1 &&s1, S2 &&s2 = "")
      : fst(std::forward<S1>(s1)), snd(std::forward<S2>(s2)) {}
};

struct VIP : public Customer {
  using Customer::Customer;
};

TEST(typeinf, jcase_positive) {
  std::cout << "--- 1. ---" << std::endl;
  Customer C("Johny", "Walker");
  std::cout << "--- 2. ---" << std::endl;
  MyString S = "Jack";
  Customer D(S, "Daniels");
  std::cout << "--- 3. ---" << std::endl;
  Customer E(std::move(S), "Daniels");
  std::cout << "--- 4. ---" << std::endl;
  Customer N("Nico");
  std::cout << "--- 5. ---" << std::endl;
  Customer M(N);
  std::cout << "--- 6. ---" << std::endl;
  VIP v2 = "BOSS";
  std::cout << "--- 7. ---" << std::endl;
  VIP v1 = v2;
  std::cout << "--- 8. ---" << std::endl;
  Customer cv = v1;
}