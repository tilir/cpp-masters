#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (lambdas lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
# Try: cmake -DCMAKE_CXX_COMPILER=clang++-15 ..
#
#------------------------------------------------------------------------------

set(SRCS
  implcast.cc
  callables.cc
  invoke.cc
  projection.cc
# generic_fail_demo.cc -- must fail for demo
  decltyped.cc
  explicit.cc
  propagated.cc
  renaming.cc
# tricky_capture_demo.cc -- must fail for demo
# transparent.cc -- this test will fail, it shows transparency problems
  more_transparent.cc
  vapack.cc
#  fwd_capture_problem.cc -- must compile but fail execution for demo

# --- for some reasons those two passes locally but not in github actions ---
# TODO: investigate
# fwd_capture.cc
# fwd_capture_astuple.cc
# ---
)

add_gtest(lambdas)
