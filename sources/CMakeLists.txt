cmake_minimum_required(VERSION 2.8)

project(grafips_sources)

add_library(grafips_sources
  gfcpu_clock_source.cpp
  gfcpu_clock_source.h
  gfcpu_source.cpp
  gfcpu_source.h
  gfgl_source.cpp
  gfgl_source.h
  gfgpu_perf_functions.cpp
  gfgpu_perf_functions.h
  gfgpu_perf_source.cpp
  gfgpu_perf_source.h
  gfimetric_source.h
  gfproc_self_source.cpp
  gfproc_self_source.h
  )

target_include_directories( grafips_sources PUBLIC . ..)

target_link_libraries(grafips_sources dl)



