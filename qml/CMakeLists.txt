cmake_minimum_required(VERSION 2.8)

project(Grafips_c)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(graph)
add_subdirectory(providers)
add_subdirectory(publishers)
add_subdirectory(subscriber)
add_subdirectory(test)

