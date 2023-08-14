#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (concepts lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
#------------------------------------------------------------------------------

set(SRCS
  # contain.cc -- broken intentionally
  eq.cc
  # eq-enableif.cc -- broken intentionally to show error processing
  # eq-constexprif.cc -- broken intentionally to show error processing
  distance-mystery.cc
  # eq-require.cc -- broken intentionally to show error processing
  minelt.cc
  # distance-too-simple.cc -- broken intentionally
  # eq-complexreq.cc -- broken intentionally to show error processing
  difference-complex-simple.cc
  explicitreq.cc
  compound.cc
  partial.cc
  strange.cc
  # impossible.cc -- intentionally broken
  distance-working.cc
)

add_gtest(concepts)
# add_demo(algo_bench)
