cmake_minimum_required(VERSION 3.18.0)

project(file-list C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")

set(srcs "block.cc") 
set(test-srcs "test.cc") 

add_library(file-list STATIC ${srcs})

add_executable(file-list-test ${test-srcs})
target_link_libraries(file-list-test file-list)
