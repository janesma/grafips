cmake_minimum_required(VERSION 2.8)

project(grafips_publishers)

find_package(Protobuf REQUIRED)

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders
  gfmetric.proto
  gfsubscriber.proto
  )

add_library(grafips_publishers
  gfmetric.h
  gfipublisher.h
  gfpublisher.h
  gfpublisher.cpp
  gfisubscriber.h
  gfsubscriber.h
  gfsubscriber.cpp
  gfsubscriber_remote.h
  gfsubscriber_remote.cpp
  ${ProtoSources}
  ${ProtoHeaders}
)

target_include_directories( grafips_publishers PUBLIC .)

target_link_libraries(grafips_publishers grafips_providers grafips_graph grafips_os protobuf)

