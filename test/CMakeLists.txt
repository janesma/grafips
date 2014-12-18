cmake_minimum_required(VERSION 2.8)

project(grafips_test)

add_executable(test_grafips
  test_cpu_provider.cpp
  test_gl_provider.cpp
  test_gpu_source.cpp
  test_main.cpp
  test_pattern.cpp
  )

target_link_libraries(test_grafips grafips_sources grafips_publishers grafips_subscriber gtest waffle-1 GL)

