cmake_minimum_required(VERSION 2.8)

project(grafips_subscriber)

set (PROTOBUF_IMPORT_DIRS "../publishers")

find_package(Protobuf REQUIRED)

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders
  gfsubscriber.proto
  )


add_library(grafips_subscriber
  gfisubscriber.h
  gfsubscriber.cpp
  gfsubscriber.h
  gfsubscriber_remote.cpp
  gfsubscriber_remote.h
  ${ProtoSources}
  ${ProtoHeaders}
)

add_dependencies(grafips_subscriber grafips_publishers)

target_include_directories( grafips_subscriber PUBLIC .)

target_link_libraries(grafips_subscriber grafips_providers grafips_publishers grafips_graph grafips_os protobuf)

