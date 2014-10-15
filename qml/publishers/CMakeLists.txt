cmake_minimum_required(VERSION 2.8)

project(grips_publishers)

add_library(grips_publishers
  metric.h
  publisher.h
)

target_include_directories( grips_publishers PUBLIC .)


