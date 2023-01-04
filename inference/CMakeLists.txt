#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples (inference lecture)
#
# Order of examples in SRCS variable corresponds to the order in the lecture.
# Examples commented out are broken (often -- intentionally).
#
# Try: cmake -DCMAKE_CXX_COMPILER=clang++-15 ..
#
#------------------------------------------------------------------------------

set(SRCS
  leftbind.cc
  rightbind.cc
  bindfail.cc
  refovr.cc
  nrvo.cc
  jcase_single.cc
  jcase_forward.cc
#  jcase_broken_fwd.cc
  jcase_deftype.cc
#  jcase_broken_deftype.cc
  jcase_positive.cc
)

set(UNIT_TARGET inference_unit)
set(TEST_TARGET inference_test)

add_executable(${UNIT_TARGET} ${SRCS})
target_include_directories(${UNIT_TARGET} PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(${UNIT_TARGET} PRIVATE ${GTEST_BOTH_LIBRARIES} Threads::Threads)
target_compile_features(${UNIT_TARGET} PRIVATE cxx_std_20)

add_test(
  NAME ${TEST_TARGET}
  COMMAND bash -c "${CMAKE_CURRENT_BINARY_DIR}/${UNIT_TARGET}"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_tests_properties(${TEST_TARGET} PROPERTIES DEPENDS ${UNIT_TARGET})