#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (constexpr lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
#------------------------------------------------------------------------------

set(SRCS
  # heavy.cc -- just to look up the assembler
  # ctconst.cc -- not a test 
  # numlimits.cc -- broken intentionally
  numlimits-fixed.cc
  # ctfield.cc -- broken intentionally
  # ifcedetails.cc -- broken intentionally
  ceval-cinit.cc
  cexpr-local.cc
  # cepred-expr.cc -- broken intentionally
  # ubsan.cc -- broken intentionally to show how it works
  ubsan-dependent.cc
  trits.cc
  cce.cc
  complex.cc
  cycle.cc
  ctstrcpy.cc
  # virtce.cc -- broken intentionally
  virtce-ok.cc
  cenew.cc
  complex-op.cc
  binparser.cc
  trits-op.cc
  mplike.cc
)

# cecollatz.cc and ctmap.cc not used yet

add_gtest(cexpr)
# add_demo(algo_bench)
