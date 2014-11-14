cmake_minimum_required(VERSION 2.8)

project(grafips_publishers)

add_library(grafips_publishers
  gfmetric.h
  gfipublisher.h
  gfpublisher.h
  gfpublisher.cpp
  gfisubscriber.h
  gfsubscriber.h
  gfsubscriber.cpp
)

target_include_directories( grafips_publishers PUBLIC .)

target_link_libraries(grafips_publishers grafips_providers grafips_graph)

