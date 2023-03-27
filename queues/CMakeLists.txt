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
  classic_queue.cc
  # classic_queue_wrong_demo.cc -- hanging queue
  # classic_queue_demo.cc -- non-hanging but contention
)

add_gtest(queues)
add_demo(classic_queue_wrong_demo)
add_demo(classic_queue_demo)
