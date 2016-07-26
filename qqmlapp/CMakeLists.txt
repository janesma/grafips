cmake_minimum_required(VERSION 2.8)

project(Grafips)

find_package(Qt5Widgets REQUIRED) 
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(${Qt5Widgets_DEFINITIONS})

qt5_add_resources(RESOURCE_ADDED resources.qrc)

add_executable(grafips 
  main.cpp
  ${UIS_HDRS}
  ${RESOURCE_ADDED} 
)

qt5_use_modules(grafips Core Gui Widgets OpenGL Quick) 

target_link_libraries(grafips
  grafips_graph
  grafips_controls_host
  grafips_subscriber
  grafips_sources
  GL pthread)


install(TARGETS grafips RUNTIME DESTINATION bin)
