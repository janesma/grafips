cmake_minimum_required(VERSION 2.8)

project(os_test)

add_executable(test_grafips_os
  test_socket.cpp
  )

target_link_libraries(test_grafips_os grafips_os gtest_main)

#add_test(NAME test_g COMMAND test_grips)