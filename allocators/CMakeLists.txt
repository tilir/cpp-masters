#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (ranges lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
# Try: cmake -DCMAKE_CXX_COMPILER=clang++-15 ..
#
#------------------------------------------------------------------------------

set(SRCS
  attempt1.cc
  attempt2.cc
  logging.cc
  freelist.cc
  shortalloc.cc
  diffallocs.cc
  # scopedalloc.cc -- problematic case, interesting CT error
  pmrexample.cc
  pmrtestres.cc
  slist.cc
)

add_gtest(allocs)
