cmake_minimum_required(VERSION 2.8)

project(grafips_publishers)

find_package(Protobuf REQUIRED)

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders
  gfmetric.proto
  gfpublisher.proto
  gfsubscriber.proto
  )

add_library(grafips_publishers
  gfipublisher.h
  gfisubscriber.h
  gfmetric.h
  gfpublisher.cpp
  gfpublisher.h
  gfpublisher_remote.cpp
  gfpublisher_remote.h
  gfsubscriber.cpp
  gfsubscriber.h
  gfsubscriber_remote.cpp
  gfsubscriber_remote.h
  ${ProtoSources}
  ${ProtoHeaders}
)

target_include_directories( grafips_publishers PUBLIC .)

target_link_libraries(grafips_publishers grafips_providers grafips_graph grafips_os protobuf)

