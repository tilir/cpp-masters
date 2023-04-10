#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (atomics lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
# Try: cmake -DCMAKE_CXX_COMPILER=clang++-15 ..
#
#------------------------------------------------------------------------------

set(SRCS
  sptr_race.cc
  sptr_mutex.cc
# atomic_counter_demo.cc
# custom_mutex_demo.cc
# inc_naive.cc -- suppose to fail
  inc_corr.cc
# atomic_twice_demo.cc
  sptr_atomic.cc
# stack_reclaim_problem.cc -- suppose to fail
)

add_gtest(atomics)
add_demo(atomic_counter_demo)
add_demo(custom_mutex_demo)
add_demo(atomic_twice_demo)