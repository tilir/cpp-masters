#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (queues lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
# Try: cmake -DCMAKE_CXX_COMPILER=clang++-15 ..
#
#------------------------------------------------------------------------------

set(SRCS
  classic_stack.cc
  classic_queue.cc
# classic_stack_wrong_demo.cc -- hanging stack
# classic_stack_demo.cc -- non-hanging
# classic_queue_demo.cc -- non-hanging bounded queue
# unbounded_queue_demo.cc -- unbounded queue
  basic_return.cc
# auto_return_wrong.cc -- must fail
  auto_return_generic.cc
  futures.cc
  marshalling.cc
  packaged.cc
  shared.cc
  latch.cc
)

add_gtest(queues)
add_demo(classic_stack_wrong_demo)
add_demo(classic_stack_demo)
add_demo(classic_queue_demo)
add_demo(classic_queue_bench)
add_demo(unbounded_queue_demo)
