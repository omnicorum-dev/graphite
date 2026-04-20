cmake_minimum_required(VERSION 3.31)
project(Graphite)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vendor/base-layer)

# Give glm its own interface target so it propagates transitively
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)

add_executable(GraphiteTesting testing/bezier_test.cpp)
target_include_directories(GraphiteTesting PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
        ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(GraphiteTesting PUBLIC BaseLayer glm)

add_library(Graphite INTERFACE)
target_include_directories(Graphite INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
        ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Graphite INTERFACE BaseLayer glm)