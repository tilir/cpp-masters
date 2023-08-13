#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (SFINAE lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
#------------------------------------------------------------------------------

set(SRCS
  lazyness.cc
  sfinae-negate.cc
  hasfoobar-naive.cc
  # logic-broken.cc -- supposed to fail, broken intentionally
  logic-fixed.cc
  hasfoobar.cc
  detected.cc
  # void-discrim.cc -- supposed to fail, broken intentionally
  discrim-custom.cc
  # enableif-naive.cc -- supposed to fail, broken intentionally
  enableif-ptrs.cc
  enableif-ints.cc
  # fireonce-naive.cc -- supposed to fail, broken intentionally
  fireonce-trampoline.cc
  mpltags.cc
  fusion-adapt.cc
)

# st.cc is not included yet on slides so omitted

add_gtest(sfinae)
# add_demo(algo_bench)
