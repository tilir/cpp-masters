#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (runtime polymorphism lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
# Try: cmake -DCMAKE_CXX_COMPILER=clang++-15 ..
#
#------------------------------------------------------------------------------

set(SRCS
  virtintf.cc
# virtcost_demo.cc -- just to look at assembly
  virtintf_crtp.cc
# crtpcost_demo.cc -- just to look at assembly
  crtp_mixins.cc
  virtual_clone_base.cc
  virtual_clone_mixin.cc
  romb_novirt.cc
  romb_throw.cc
  visible_base.cc
  init_seq.cc
# virtinh_cost_demo.cc -- just to look at assembly
  dynamic_casts.cc
# dyncast_cost_demo.cc -- just to look at assembly
  discouraging_function.cc
# sean_parents.cc -- executable demo
)

add_gtest(runtimepoly)
