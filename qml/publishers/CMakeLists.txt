cmake_minimum_required(VERSION 2.8)

project(grafips_publishers)

find_package(Protobuf REQUIRED)

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders
  gfmetric.proto
  gfpublisher.proto
  )


add_library(grafips_publishers
  gfipublisher.h
  gfmetric.h
  gfpublisher.cpp
  gfpublisher.h
  gfpublisher_skel.cpp
  gfpublisher_skel.h
  gfpublisher_remote.cpp
  gfpublisher_remote.h
  ${ProtoSources}
  ${ProtoHeaders}
)


target_include_directories( grafips_publishers PUBLIC . ../subscriber ${CMAKE_BINARY_DIR}/qml/publishers )

target_link_libraries(grafips_publishers grafips_subscriber grafips_providers grafips_os Qt5::Core Qt5::Quick protobuf)

