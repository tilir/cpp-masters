//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// custom shared ptr atomic
//
//----------------------------------------------------------------------------

#include <future>
#include <iostream>
#include <latch>
#include <thread>
#include <utility>

#include "gtest/gtest.h"

namespace {

// control block for shared_pointer
template <typename T> struct CBlock {
  T *Data = nullptr;
  std::atomic<int> Counter = 1;
  CBlock(const T &Data) : Data(new T(Data)) {}
  int add(int N) {
    Counter += N;
    return Counter;
  }
};

// shared_pointer itself
template <typename T> class MySPTR {
  CBlock<T> *B;

public:
  MySPTR(const T &t) : B(new CBlock(t)) {}
  MySPTR(const MySPTR &Rhs) {
    B = Rhs.B;
    B->add(1);
  }
  ~MySPTR() {
    if (B->add(-1) == 0)
      delete B;
  }
  T *operator->() const { return B->Data; }
  T &operator*() const { return *B->Data; }
};

} // namespace

TEST(atomics, sptr_atomic) {
  MySPTR<int> S(42);
  std::jthread Fst([S] { MySPTR<int> S1{S}; });
  std::jthread Snd([S] { MySPTR<int> S1{S}; });
  EXPECT_EQ(*S, 42);
}