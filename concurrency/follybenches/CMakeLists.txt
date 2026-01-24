cmake_minimum_required(VERSION 3.16)
project(rcu_example CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(gflags CONFIG REQUIRED)
find_package(folly CONFIG REQUIRED)

add_executable(mutex_bench mutex_bench.cc)
target_link_libraries(mutex_bench PRIVATE Folly::folly Folly::follybenchmark gflags)

