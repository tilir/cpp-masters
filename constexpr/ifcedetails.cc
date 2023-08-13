//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// if constexpr usage: note only instantiations are affected
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <type_traits>

// template case: constexpr works

template <typename T> void bar(T value) {
  if constexpr (std::is_pointer_v<T>)
    std::cout << "Ptr to " << *value << std::endl; // Error
  else
    std::cout << "Ref to " << value << std::endl;
}

template void bar<int>(int);     // instantiate here!
template void bar<int *>(int *); // instantiate here!

// non-template case: constexpr fails

void foo() {
  auto value = 100; // deduce int
  if constexpr (std::is_pointer_v<decltype(value)>)
    std::cout << "Ptr to " << *value << std::endl; // Error
  else
    std::cout << "Ref to " << value << std::endl;
}