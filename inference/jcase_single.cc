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
// Initial case: old good const lvalue refs.
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

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
  Customer(const MyString &s1, const MyString &s2) : fst(s1), snd(s2) {}
};

TEST(typeinf, jcase_single) {
  std::cout << "--- 1. ---" << std::endl;
  Customer C("Johny", "Walker");
  std::cout << "--- 2. ---" << std::endl;
  MyString S = "Jack";
  Customer D(S, "Daniels");
  std::cout << "--- 3. ---" << std::endl;
  Customer E(std::move(S), "Daniels");
}