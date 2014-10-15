cmake_minimum_required(VERSION 2.8)

project(Grips)

find_package(Qt5Widgets REQUIRED) 
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES( )

add_definitions(${Qt5Widgets_DEFINITIONS})

QT5_WRAP_UI(UIS_HDRS
#  mainwindow.ui
)

qt5_add_resources(RESOURCE_ADDED resources.qrc)

add_executable(grips 
  main.cpp
  data_sink.h
  gripsrenderer.h
  gripsrenderer.cpp
  ${UIS_HDRS}
  ${RESOURCE_ADDED} 
)

qt5_use_modules(grips Core Gui Widgets OpenGL Quick Declarative) 

target_link_libraries(grips grips_providers GL)

add_subdirectory(test)
add_subdirectory(providers)
add_subdirectory(/usr/src/gtest gtest)

