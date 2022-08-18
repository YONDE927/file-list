cmake_minimum_required(VERSION 3.18.0)

project(file-list C CXX)

set(srcs "block.cc") 

add_library(file-list STATIC ${srcs})
