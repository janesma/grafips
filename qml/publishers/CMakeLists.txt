cmake_minimum_required(VERSION 2.8)

project(grips_publishers)

add_library(grips_publishers
  metric.h
  gfipublisher.h
  publisher.h
  publisher.cpp
  subscriber.h
  subscriber.cpp
)

target_include_directories( grips_publishers PUBLIC .)

target_link_libraries(grips_publishers grips_providers grips_graph)

