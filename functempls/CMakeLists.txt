#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (function templates lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
#------------------------------------------------------------------------------

set(SRCS
  nth-power-concrete.cc
  # nth-power-naive.cc -- supposed to fail
  nth-power-traits.cc
  nth-power-clean.cc
  nth-power-concept.cc
  class-typename.cc
  templ-inst-spec.cc
)

add_gtest(functempls)
# add_demo(algo_bench)
