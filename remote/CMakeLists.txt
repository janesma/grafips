cmake_minimum_required(VERSION 2.8)

# this library should contain all of the files that fips needs to link
# against.  There should be no Qt dependency 
project(grafips_remote)

find_package(Protobuf REQUIRED)

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders
  gfmetric.proto
  gfpublisher.proto
  gfsubscriber.proto
  )


add_library(grafips_remote
  ${ProtoHeaders}
  ${ProtoSources}
  gfipublisher.h
  gfisubscriber.h
  gfmetric.h
  gfpublisher.cpp
  gfpublisher.h
  gfpublisher_skel.cpp
  gfpublisher_skel.h
  gfsubscriber_stub.cpp
  gfsubscriber_stub.h
)


target_include_directories( grafips_remote PUBLIC . ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/remote)

target_link_libraries(grafips_remote grafips_providers grafips_os protobuf)

