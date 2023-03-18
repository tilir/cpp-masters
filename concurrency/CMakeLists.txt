#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (concurrency lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
# Try: cmake -DCMAKE_CXX_COMPILER=clang++-15 ..
#
#------------------------------------------------------------------------------

set(SRCS
  thread_create.cc
  spawned_id.cc
# race_demo.cc -- we have UB
# race_demo_volatile.cc -- we have UB
  volatile.cc
# posix_solution.cc -- we have UB
# locations_ub.cc -- we have UB
  locations_ok.cc
  mutex_solution.cc
  lock_guard.cc
  sutter_case.cc
# deadlock.cc
  dining_naive.cc
  dining_lock.cc
  dcl.cc
  dcl_once.cc
  notify_one.cc
  notify_all.cc
# focus_demo.cc -- just demo
# shared_lock_bench.cc -- benchmark
)

add_gtest(concurrency)
