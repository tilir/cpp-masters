cmake_minimum_required(VERSION 3.16)
project(rcu_example CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(gflags CONFIG REQUIRED)
find_package(folly CONFIG REQUIRED)

add_executable(rcu_example rcu.cpp)
add_executable(hazard_example hazard.cpp)
target_link_libraries(rcu_example PRIVATE Folly::folly)
target_link_libraries(hazard_example PRIVATE Folly::folly)

