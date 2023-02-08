#------------------------------------------------------------------------------
#
# Source code for MIPT masters course on C++
# Slides: https://sourceforge.net/projects/cpp-lects-rus
# Licensed after GNU GPL v3
#
#------------------------------------------------------------------------------
#
# Build system for examples to all masters C++ lectures.
# Order of subdirs is order of lectures in the course
#
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.11)
project(cppmasters)

find_package(GTest REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

enable_testing()

function(add_gtest TGT)
  set(UNIT_TARGET ${TGT}_unit)
  set(TEST_TARGET ${TGT}_test)
   
  add_executable(${UNIT_TARGET} ${SRCS})
  target_include_directories(${UNIT_TARGET} PRIVATE ${GTEST_INCLUDE_DIRS})
  target_link_libraries(${UNIT_TARGET} PRIVATE ${GTEST_BOTH_LIBRARIES} Threads::Threads)
  target_compile_features(${UNIT_TARGET} PRIVATE cxx_std_20)

  add_test(
    NAME ${TEST_TARGET}
    COMMAND ${UNIT_TARGET}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  set_tests_properties(${TEST_TARGET} PROPERTIES DEPENDS ${UNIT_TARGET})
endfunction()

# add_subdirectory(strings) -- in progress
# add_subdirectory(functempls) -- in progress
add_subdirectory(classtemplates)
# add_subdirectory(modules) -- not simple with cmake
# add_subdirectory(sfinae) -- in progress
# add_subdirectory(constexpr)
# add_subdirectory(concepts)
add_subdirectory(inference)
add_subdirectory(variadics)
# add_subdirectory(lambdas) -- in progress
add_subdirectory(ranges)
add_subdirectory(allocators)

install()
