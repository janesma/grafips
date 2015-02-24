cmake_minimum_required(VERSION 2.8)


project(grafips_os)

add_library(grafips_os
  gfthread.h
  gfthread.cpp
  gfsocket.h
  gfsocket.cpp
  gftraits.h
  gfmutex.h
  gfmutex.cpp
  )

target_include_directories( grafips_os PUBLIC ..)

target_link_libraries(grafips_os grafips_error)

add_subdirectory(test)


