cmake_minimum_required(VERSION 2.8)

project(grips_providers)

add_library(grips_providers
  cpu_provider.h
  cpu_provider.cpp
  provider.h
  provider.cpp
)

target_include_directories( grips_providers PUBLIC .)

target_link_libraries(grips_providers grips_publishers)



