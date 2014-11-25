cmake_minimum_required(VERSION 2.8)

project(grafips_publishers)

add_library(grafips_publishers
  gfpublisher_remote.cpp
  gfpublisher_remote.h
)


target_include_directories( grafips_publishers PUBLIC . )

target_link_libraries(grafips_publishers
  grafips_os
  grafips_remote
  grafips_providers
  grafips_subscriber
  Qt5::Core Qt5::Quick
  protobuf)

