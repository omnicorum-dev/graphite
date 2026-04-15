cmake_minimum_required(VERSION 3.31)
project(Graphite)

set(CMAKE_CXX_STANDARD 17)

add_executable(Graphite testing/main.cpp)
include_directories(base-layer .)