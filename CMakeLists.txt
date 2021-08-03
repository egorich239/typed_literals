cmake_minimum_required(VERSION 3.20)
project(typed_literals)

set(CMAKE_CXX_STANDARD 17)

add_library(typed_literals INTERFACE)
target_include_directories(typed_literals INTERFACE .)
