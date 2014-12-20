cmake_minimum_required(VERSION 2.8)

project(grafips_sources)

add_library(grafips_sources
  gfcpu_source.cpp
  gfcpu_source.h
  gfgl_source.cpp
  gfgl_source.h
  gfgpu_perf_source.cpp
  gfgpu_perf_source.h
  gfimetric_source.h
)

target_include_directories( grafips_sources PUBLIC .)

target_link_libraries(grafips_sources grafips_publishers GL)



