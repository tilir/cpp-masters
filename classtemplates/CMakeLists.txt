#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (class templates lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
# Try: cmake -DCMAKE_CXX_COMPILER=clang++-15 ..
#
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.11)
project(classtempls)

find_package(GTest REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SRCS
  point-of-decl.cc
# danger-1.cc -- requires separate build
# danger-2.cc -- requires separate build
  partial-primer.cc
  backward.cc
  partial-func.cc
  member-func.cc
  member-func-spec.cc
  forint.cc
  dedguide.cc
  dedsimple.cc
)

add_executable(classtempls_tests ${SRCS})
target_include_directories(classtempls_tests PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(classtempls_tests PRIVATE ${GTEST_BOTH_LIBRARIES} Threads::Threads)
target_compile_features(classtempls_tests PRIVATE cxx_std_20)