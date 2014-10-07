cmake_minimum_required(VERSION 2.8)

project(Grips)

find_package(Qt5Widgets REQUIRED) 
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)


# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${OPENGL_INCLUDE_DIR})

add_definitions(${Qt5Widgets_DEFINITIONS})

QT5_WRAP_UI(UIS_HDRS
  mainwindow.ui
)

#qt5_add_resources(RESOURCE_ADDED resources.qrc)

add_executable(grips 
  main.cpp
  mainwindow.h
  mainwindow.cpp
  glwidget.h
  glwidget.cpp
  ${UIS_HDRS}
  ${RESOURCE_ADDED} 
)

qt5_use_modules(grips Core Gui Widgets OpenGL Quick Declarative) 

target_link_libraries(grips GL
    )

