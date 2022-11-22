//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// exotic fold example
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

template <typename T> struct Node {
  T data;
  Node *left = nullptr;
  Node *right = nullptr;
};

template <class T, class... Args>
Node<T> *tree_get(Node<T> *top, Args... args) {
  return (top->*...->*args);
}

TEST(variadic, exoticfold) {
  Node<int> t[5];
  t[0].left = &t[1];
  t[1].left = &t[2];
  t[2].right = &t[3];
  t[3].data = 5;
  auto left = &Node<int>::left;
  auto right = &Node<int>::right;
  auto *res = tree_get(&t[0], left, left, right);
  EXPECT_EQ(res->data, 5);
}