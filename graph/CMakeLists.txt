cmake_minimum_required(VERSION 2.8)

project(grafips_graph)

add_library(grafips_graph
  gfgraph_set.h
  gfgraph_set.cpp
  gfgraph_view.h
  gfgraph_view.cpp
)

target_include_directories( grafips_graph PUBLIC .. )

target_link_libraries(grafips_graph grafips_subscriber grafips_publishers Qt5::OpenGL Qt5::Quick)



