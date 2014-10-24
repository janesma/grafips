cmake_minimum_required(VERSION 2.8)

project(GripsTest)

add_executable(test_grips
  test_cpu_provider.cpp
  )

target_link_libraries(test_grips grips_providers grips_publishers gtest_main)

add_test(NAME test_g COMMAND test_grips)