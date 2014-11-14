cmake_minimum_required(VERSION 2.8)

project(grafips_test)

add_executable(test_grafips
  test_cpu_provider.cpp
  )

target_link_libraries(test_grafips grafips_providers grafips_publishers gtest_main)

add_test(NAME test_g COMMAND test_grafips)