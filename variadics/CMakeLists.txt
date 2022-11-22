#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (variadic lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
# Try: cmake -DCMAKE_CXX_COMPILER=clang++-15 ..
#
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.11)
project(variadic)

find_package(GTest REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SRCS
  cstyle.cc
#  cxxstyle_wrong.cc
  cxxstyle_ok.cc
  cxxstyle_another_ok.cc
  expansion.cc
#  expansion_wrong.cc
  expansion_initlist.cc
  expansion_proxy.cc
  commas.cc
  mixins.cc
  twopacks.cc
  exoticfold.cc
  voidpack.cc
  sumall.cc
  reqpack.cc
  conj.cc
  addable.cc
#  addable2.cc
  ineff.cc
  moreeff.cc
  realeff.cc
)

add_executable(variadic_tests ${SRCS})
target_include_directories(variadic_tests PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(variadic_tests PRIVATE ${GTEST_BOTH_LIBRARIES} Threads::Threads)
target_compile_features(variadic_tests PRIVATE cxx_std_20)