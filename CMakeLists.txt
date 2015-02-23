cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

project(top_level)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")

find_package(Qt5Core REQUIRED)
find_package(Qt5Declarative REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Widgets REQUIRED) 
find_package(Waffle REQUIRED)


set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(/usr/src/gtest gtest)
add_subdirectory(controls)
add_subdirectory(error)
add_subdirectory(graph)
add_subdirectory(os)
add_subdirectory(publishers)
add_subdirectory(qqmlapp)
add_subdirectory(remote)
add_subdirectory(sources)
add_subdirectory(standalone)
add_subdirectory(subscriber)
add_subdirectory(test)
