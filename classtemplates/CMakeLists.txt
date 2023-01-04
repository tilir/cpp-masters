#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (class templates lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
#------------------------------------------------------------------------------

set(SRCS
  point-of-decl.cc
# danger-1.cc -- requires separate build
# danger-2.cc -- requires separate build
  partial-primer.cc
  backward.cc
  partial-func.cc
  member-func.cc
  member-func-spec.cc
  forint.cc
  dedguide.cc
  dedsimple.cc
)

add_gtest(classtempls)