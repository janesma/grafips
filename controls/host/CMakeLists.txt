cmake_minimum_required(VERSION 2.8)

project(grafips_controls_host)

add_library(grafips_controls_host
  gfcontrol_host.h
  gfcontrol_host.cpp
  gfcpu_freq_qcontrol.h
  gfcpu_freq_qcontrol.cpp
  )

target_include_directories( grafips_controls_host PUBLIC ../..)

target_link_libraries(grafips_controls_host
  grafips_controls
  grafips_os
  Qt5::Core
  )


