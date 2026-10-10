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

cmake_minimum_required(VERSION 3.21)
# Respect an explicit compiler or CXX; otherwise use the course default.
if(NOT DEFINED CMAKE_CXX_COMPILER AND NOT DEFINED ENV{CXX})
  set(CMAKE_CXX_COMPILER g++-15 CACHE STRING "C++ compiler")
endif()
project(cppmasters LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#------------------------------------------------------------------------------
# Optional include-what-you-use integration:
#   cmake -DCPM_ENABLE_IWYU=ON ...
# See tools/iwyu.rb for a standalone (no-reconfigure) driver.
#------------------------------------------------------------------------------
option(CPM_ENABLE_IWYU "Build with include-what-you-use checks" OFF)
if(CPM_ENABLE_IWYU)
  find_program(CPM_IWYU_EXE NAMES include-what-you-use iwyu
               HINTS ${CMAKE_CURRENT_SOURCE_DIR}/build/tools/iwyu/bin REQUIRED)
  execute_process(COMMAND ${CPM_IWYU_EXE} --version
                  OUTPUT_VARIABLE CPM_IWYU_VERSION
                  RESULT_VARIABLE CPM_IWYU_STATUS)
  if(NOT CPM_IWYU_STATUS EQUAL 0 OR
     NOT CPM_IWYU_VERSION MATCHES "0\\.25 .*clang version 21\\.")
    message(FATAL_ERROR "IWYU 0.25 built against Clang 21 is required")
  endif()
  set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE
      ${CPM_IWYU_EXE}
      -Xiwyu --mapping_file=${CMAKE_CURRENT_SOURCE_DIR}/tools/iwyu.imp
      -Xiwyu --no_comments
      -Xiwyu --no_fwd_decls
      -Xiwyu --check_also=${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
      -Xiwyu --error)
endif()

#------------------------------------------------------------------------------
# Atomics on small structs (e.g. atomic<Counters> in atomics/inc_corr.cc)
# need libatomic on some toolchains: clang + libstdc++ lowers their
# compare_exchange to the generic __atomic_compare_exchange libcall.
# Link libatomic only where the compiler cannot inline it.
#------------------------------------------------------------------------------
include(CheckCXXSourceCompiles)
check_cxx_source_compiles(
  "
#include <atomic>
struct Counters { int a = 0, b = 0; };
std::atomic<Counters> c;
int main() {
  Counters oldc = c.load(), newc{oldc.a + 1, oldc.b + 1};
  return c.compare_exchange_weak(oldc, newc) ? 0 : 1;
}"
  CPM_HAVE_BUILTIN_WIDE_ATOMICS)

add_library(cpm_atomic INTERFACE)
if(NOT CPM_HAVE_BUILTIN_WIDE_ATOMICS)
  target_link_libraries(cpm_atomic INTERFACE atomic)
endif()

add_library(commonutils INTERFACE)
target_include_directories(commonutils INTERFACE utils)

# Common warning set for all examples. No -Werror: some examples trigger
# warnings on purpose (suppressed per-target with a comment where needed).
add_library(cpm_warnings INTERFACE)
target_compile_options(cpm_warnings INTERFACE -Wall -Wextra)

enable_testing()

function(add_gtest TGT)
  set(UNIT_TARGET ${TGT}_unit)
  set(TEST_TARGET ${TGT}_test)

  add_executable(${UNIT_TARGET} ${SRCS})
  target_include_directories(${UNIT_TARGET} PRIVATE ${GTEST_INCLUDE_DIRS})
  target_link_libraries(${UNIT_TARGET} PRIVATE ${GTEST_BOTH_LIBRARIES})
  target_link_libraries(${UNIT_TARGET} PRIVATE Threads::Threads)
  target_link_libraries(${UNIT_TARGET} PRIVATE commonutils)
  target_link_libraries(${UNIT_TARGET} PRIVATE cpm_atomic)
  target_link_libraries(${UNIT_TARGET} PRIVATE cpm_warnings)
  target_compile_features(${UNIT_TARGET} PRIVATE cxx_std_20)

  add_test(
    NAME ${TEST_TARGET}
    COMMAND ${UNIT_TARGET}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  set_tests_properties(${TEST_TARGET} PROPERTIES DEPENDS ${UNIT_TARGET})
endfunction()

function(add_demo TGT)
  add_executable(${TGT} ${TGT}.cc)
  target_include_directories(${TGT} PRIVATE ${GTEST_INCLUDE_DIRS})
  target_link_libraries(${TGT} PRIVATE Threads::Threads)
  target_link_libraries(${TGT} PRIVATE cpm_atomic)
  target_link_libraries(${TGT} PRIVATE commonutils)
  target_link_libraries(${TGT} PRIVATE cpm_warnings)
  target_compile_features(${TGT} PRIVATE cxx_std_20)
endfunction()

add_subdirectory(strings)
add_subdirectory(functempls)
add_subdirectory(classtemplates)
# add_subdirectory(modules) -- not simple with cmake
add_subdirectory(sfinae)
add_subdirectory(constexpr)
add_subdirectory(concepts)
add_subdirectory(inference)
add_subdirectory(variadics)
add_subdirectory(lambdas)
add_subdirectory(ranges)
add_subdirectory(allocators)
add_subdirectory(smartptrs)
add_subdirectory(runtimepoly)
add_subdirectory(concurrency)
add_subdirectory(queues)
add_subdirectory(atomics)
add_subdirectory(parallel)
add_subdirectory(coroutines)
