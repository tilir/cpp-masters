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
  teaser.cc
# errmsg.cc
  proj.cc 
  dangling.cc
# typed.cc -- requires separate build
  endvalue.cc
  fibiter.cc
  counted.cc
  sentinel.cc
  common.cc
  subsort.cc
  unreachable.cc
  takemore.cc
  strangeissue.cc
  sfinaeissue.cc
  nonsfinaeissue.cc
  transformed.cc
# danglemessage.cc
  nondangleview.cc
  notlazy.cc
  simplecomb.cc
# constfilter.cc
  pullmodel.cc
# brokenfilter.cc 
  tostr_view.cc
  tostr_adaptor.cc
# trimnaive.cc -- segfaults for gcc 11
  trimbetter.cc
# buggy-concat.cc -- requires separate build
# working-concat.cc -- requires separate build
)

add_gtest(ranges)