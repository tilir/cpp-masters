//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Spedcialized member template function
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

template <> template <> std::string DataReader<Data>::read<std::string>() {
  return "S";
}

TEST(fullspec, memfunc) {
  Data d;
  DataReader<Data> reader(d);
  auto myval = reader.read<int>();
  EXPECT_EQ(myval, 0);
  auto mys = reader.template read<std::string>();
  EXPECT_EQ(mys, "S");
}