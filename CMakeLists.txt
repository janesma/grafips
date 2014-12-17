cmake_minimum_required(VERSION 2.8)

project(top_level)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")

find_package(Qt5Widgets REQUIRED) 
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Declarative REQUIRED)
find_package(X11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(graph)
add_subdirectory(sources)
add_subdirectory(publishers)
add_subdirectory(subscriber)
add_subdirectory(test)
add_subdirectory(os)
add_subdirectory(qqmlapp)
add_subdirectory(remote)
add_subdirectory(standalone)
add_subdirectory(/usr/src/gtest gtest)
