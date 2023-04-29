#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (coroutines lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
#------------------------------------------------------------------------------

set(SRCS
  natseq.cc
  resumable.cc
  natseq_range.cc
  natseq_zip.cc
  subscribers.cc
#  subscribers_mt.cc -- shall fail
  subscribers_natlst.cc
  cancellable.cc
  futures.cc
)

add_gtest(coroutines)
# add_demo(algo_bench)
