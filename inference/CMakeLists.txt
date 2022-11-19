#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (inference lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
# Try: cmake -DCMAKE_CXX_COMPILER=clang++-15 ..
#
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.11)
project(inference)

find_package(GTest REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SRCS
  leftbind.cc
  rightbind.cc
  bindfail.cc
  refovr.cc
  nrvo.cc
  jcase_single.cc
  jcase_forward.cc
#  jcase_broken_fwd.cc
  jcase_deftype.cc
#  jcase_broken_deftype.cc
  jcase_positive.cc
)

add_executable(inference_tests ${SRCS})
target_include_directories(inference_tests PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(inference_tests PRIVATE ${GTEST_BOTH_LIBRARIES} Threads::Threads)
target_compile_features(inference_tests PRIVATE cxx_std_20)