cmake_minimum_required(VERSION 3.31)
project(Graphite)

set(CMAKE_CXX_STANDARD 17)

add_executable(app testing/main.cpp)
target_include_directories(app PUBLIC vendor/base-layer vendor ./)

add_library(Graphite)
target_sources(Graphite PRIVATE graphite.cpp)
target_include_directories(Graphite PUBLIC vendor/base-layer vendor ./)