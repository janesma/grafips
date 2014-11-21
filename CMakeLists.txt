cmake_minimum_required(VERSION 2.8)

project(top_level)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

find_package(Qt5Widgets REQUIRED) 
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Declarative REQUIRED)

add_subdirectory(qml)
add_subdirectory(os)
add_subdirectory(qqmlapp)
add_subdirectory(standalone)
add_subdirectory(/usr/src/gtest gtest)
