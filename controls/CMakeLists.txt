cmake_minimum_required(VERSION 2.8)

project(grafips_controls)

find_package(Protobuf REQUIRED)

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders
  gfcontrol.proto
  )

add_library(grafips_controls
  ${ProtoHeaders}
  ${ProtoSources}
  gficontrol.h
  gfcontrol.h
  gfcontrol.cpp
  gfcpu_freq_control.cpp
  gfcpu_freq_control.h
  gfcontrol_stub.h
  gfcontrol_stub.cpp
  )

target_include_directories( grafips_controls PUBLIC .. ${CMAKE_BINARY_DIR}/controls)

#target_link_libraries(grafips_controls)



