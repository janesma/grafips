cmake_minimum_required(VERSION 2.8)

project(grips_graph)

add_library(grips_graph
  graph_set.h
  graph_set.cpp
)

target_include_directories( grips_graph PUBLIC .)

target_link_libraries(grips_graph grips_publishers)



