#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (parallelism lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
#------------------------------------------------------------------------------

set(SRCS
  paralg.cc
  my_reduce.cc
  my_reduce_pkg.cc
  lambdas_async.cc
  my_reduce_async.cc
)

add_gtest(parallel)
add_demo(algo_bench)
add_demo(reduce_bench)

# std::execution::par with the libstdc++ PSTL backend needs TBB at link time
# (only when TBB headers are installed -- otherwise PSTL falls back to serial).
find_package(TBB QUIET)
if(TBB_FOUND)
  foreach(tgt parallel_unit algo_bench reduce_bench)
    target_link_libraries(${tgt} PRIVATE TBB::tbb)
  endforeach()
endif()