cmake_minimum_required(VERSION 2.8)


project(grafips_os)

add_library(grafips_os
  gfthread.h
  gfthread.cpp
  gfsocket.h
  gfsocket.cpp
  gftraits.h
  )

target_include_directories( grafips_os PUBLIC .)

add_subdirectory(test)


