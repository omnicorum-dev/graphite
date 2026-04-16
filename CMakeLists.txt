cmake_minimum_required(VERSION 3.31)
project(Graphite)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vendor/base-layer)

add_executable(GraphiteTesting testing/main.cpp)
target_include_directories(GraphiteTesting PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
        ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(GraphiteTesting PUBLIC BaseLayer)

add_library(Graphite INTERFACE)
target_include_directories(Graphite INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
        ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Graphite INTERFACE BaseLayer)