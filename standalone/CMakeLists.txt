cmake_minimum_required(VERSION 2.8)

project(standalone)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

add_executable(grafips_publish 
  main.cpp
)

target_link_libraries(grafips_publish grafips_test_context grafips_sources grafips_publishers grafips_subscriber waffle-1 pthread)
