cmake_minimum_required(VERSION 2.8)

project(grafips_subscriber)

add_library(grafips_subscriber
  gfsubscriber.cpp
  gfsubscriber.h
  gfsubscriber_remote.cpp
  gfsubscriber_remote.h
)

add_dependencies(grafips_subscriber grafips_publishers)

target_include_directories( grafips_subscriber PUBLIC .)

target_link_libraries(grafips_subscriber grafips_sources grafips_publishers grafips_remote grafips_graph grafips_os protobuf)

