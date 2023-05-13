#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (strings lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
#------------------------------------------------------------------------------

set(SRCS
  hello.cc
  conversions.cc
# cstrs.cc -- do not compile  
# fmt.cc -- requires GCC 13
  tokenizer.cc
)

add_gtest(strings)
# add_demo(algo_bench)
