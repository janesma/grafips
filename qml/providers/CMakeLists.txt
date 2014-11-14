cmake_minimum_required(VERSION 2.8)

project(grafips_providers)

add_library(grafips_providers
  gfcpu_provider.h
  gfcpu_provider.cpp
  gfprovider.h
  gfprovider.cpp
)

target_include_directories( grafips_providers PUBLIC .)

target_link_libraries(grafips_providers grafips_publishers)



