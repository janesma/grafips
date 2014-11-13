cmake_minimum_required(VERSION 2.8)

project(top_level)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

add_subdirectory(qml)
add_subdirectory(os)
add_subdirectory(qqmlapp)
add_subdirectory(/usr/src/gtest gtest)
