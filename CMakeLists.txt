cmake_minimum_required(VERSION 3.10)
project(A-plus-B)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/)

add_executable(code solution.cpp)