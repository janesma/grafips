cmake_minimum_required(VERSION 2.8)

project(GripsTest)

add_executable(test_grips
  test_cpu_provider.cpp
  )

target_link_libraries(test_grips grips_providers gtest_main)
