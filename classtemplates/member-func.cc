//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Member template function and twice-template
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

struct Data {
  template <typename T> T read() const { return {}; }
};

template <typename T> class DataReader {
  const T &source_;

public:
  DataReader(const T &source) : source_(source) {}
  template <typename R> R read();
};

template <typename T> template <typename R> R DataReader<T>::read() {
  R res = source_.template read<R>();
  return res;
}

TEST(classtempls, memfunc) {
  Data d;
  DataReader<Data> reader(d);
  auto myval = reader.read<int>();
  EXPECT_EQ(myval, 0);
}