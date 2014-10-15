cmake_minimum_required(VERSION 2.8)

project(grips_providers)

add_library(grips_providers
  cpu_provider.h
  cpu_provider.cpp
  provider.h
)

target_include_directories( grips_providers PUBLIC .)


