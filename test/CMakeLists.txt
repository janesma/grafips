cmake_minimum_required(VERSION 2.8)

project(grafips_test)

add_library(grafips_test_context
  test_gpu_context.cpp
  )

target_include_directories( grafips_test_context PUBLIC .. )
target_link_libraries(grafips_test_context GL waffle-1)

add_executable(test_grafips
  test_cpu_freq_control.cpp
  test_cpu_provider.cpp
  test_error.cpp
  test_gl_provider.cpp
  test_gpu_source.cpp
  test_main.cpp
  test_pattern.cpp
  test_proc_self_source.cpp
  )

target_link_libraries(test_grafips
  grafips_controls
  grafips_controls_host
  grafips_error
  grafips_publishers
  grafips_remote
  grafips_sources
  grafips_subscriber
  grafips_test_context
  gtest waffle-1 GL)

