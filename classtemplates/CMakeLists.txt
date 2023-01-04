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

set(UNIT_TARGET classtempls_unit)
set(TEST_TARGET classtempls_test)

add_executable(${UNIT_TARGET} ${SRCS})
target_include_directories(${UNIT_TARGET} PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(${UNIT_TARGET} PRIVATE ${GTEST_BOTH_LIBRARIES} Threads::Threads)
target_compile_features(${UNIT_TARGET} PRIVATE cxx_std_20)

add_test(
  NAME ${TEST_TARGET}
  COMMAND bash -c "${CMAKE_CURRENT_BINARY_DIR}/${UNIT_TARGET}"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_tests_properties(${TEST_TARGET} PROPERTIES DEPENDS ${UNIT_TARGET})