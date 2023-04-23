//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// really effective emplace
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <iostream>

struct Heavy {
  Heavy(int sz) { std::cout << "Heavy created" << std::endl; }
  Heavy(const Heavy &rhs) { std::cout << "Heavy copied" << std::endl; }
  ~Heavy() { std::cout << "Heavy destroyed" << std::endl; }
};

template <typename T> class Stack {
  struct StackElem {
    T elem;
    StackElem *next;
    StackElem(StackElem *nxt, T e) : elem(e), next(nxt) {}

    template <class... Args>
    StackElem(StackElem *nxt, Args &&...args)
        : elem(std::forward<Args>(args)...), next(nxt) {}
  };

  struct StackElem *top_;

public:
  Stack() : top_(nullptr) {}
  Stack(const Stack<T> &rhs) = delete;
  ~Stack();

  Stack<T> &operator=(const Stack<T> &rhs) = delete;

  void push_back(const T &);

  template <class... Args> void emplace_back(Args &&...args);
};

template <typename T> void Stack<T>::push_back(const T &elem) {
  StackElem *newelem = new StackElem(elem, top_);
  top_ = newelem;
}

template <typename T>
template <class... Args>
void Stack<T>::emplace_back(Args &&...args) {
  StackElem *newelem = new StackElem(top_, std::forward<Args>(args)...);
  top_ = newelem;
}

template <typename T> Stack<T>::~Stack() {
  struct StackElem *nxt = top_;
  while (nxt != nullptr) {
    struct StackElem *tmp = nxt->next;
    delete nxt;
    nxt = tmp;
  }
  top_ = nullptr;
}

TEST(variadic, realeff) {
  Stack<Heavy> s;
  for (int i = 0; i != 10; ++i) {
    std::cout << std::endl << "next" << std::endl;
    s.emplace_back(100);
  }
  EXPECT_TRUE(true);
}