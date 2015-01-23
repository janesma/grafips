cmake_minimum_required(VERSION 2.8)

project(grafips_publishers)

add_library(grafips_publishers
  gfmetric_router.cpp
  gfmetric_router.h
  gfpattern.cpp
  gfpattern.h
  gfpublisher_remote.cpp
  gfpublisher_remote.h
  )


target_include_directories( grafips_publishers PUBLIC ../.. )

target_link_libraries(grafips_publishers
  grafips_os
  grafips_remote
  grafips_sources
  grafips_subscriber
  Qt5::Core
  protobuf)

