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

install()