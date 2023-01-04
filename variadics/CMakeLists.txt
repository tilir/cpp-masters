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

set(UNIT_TARGET variadic_unit)
set(TEST_TARGET variadic_test)

add_executable(${UNIT_TARGET} ${SRCS})
target_include_directories(${UNIT_TARGET} PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(${UNIT_TARGET} PRIVATE ${GTEST_BOTH_LIBRARIES} Threads::Threads)
target_compile_features(${UNIT_TARGET} PRIVATE cxx_std_20)

add_test(
  NAME ${TEST_TARGET}
  COMMAND bash -c "${CMAKE_CURRENT_BINARY_DIR}/${UNIT_TARGET}"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_tests_properties(${TEST_TARGET} PROPERTIES DEPENDS ${UNIT_TARGET})