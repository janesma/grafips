cmake_minimum_required(VERSION 2.8)

project(grafips_error)

add_library(grafips_error
  gferror.h
  gferror.cpp
  )

target_include_directories( grafips_error PUBLIC ..)

