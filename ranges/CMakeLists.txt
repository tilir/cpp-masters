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

set(UNIT_TARGET ranges_unit)
set(TEST_TARGET ranges_test)

add_executable(${UNIT_TARGET} ${SRCS})
target_include_directories(${UNIT_TARGET} PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(${UNIT_TARGET} PRIVATE ${GTEST_BOTH_LIBRARIES} Threads::Threads)
target_compile_features(${UNIT_TARGET} PRIVATE cxx_std_20)

add_test(
  NAME ${TEST_TARGET}
  COMMAND bash -c "${CMAKE_CURRENT_BINARY_DIR}/${UNIT_TARGET}"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_tests_properties(${TEST_TARGET} PROPERTIES DEPENDS ${UNIT_TARGET})