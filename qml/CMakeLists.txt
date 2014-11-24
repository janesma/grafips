cmake_minimum_required(VERSION 2.8)

project(Grafips_c)



# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES( )

# add_definitions(${Qt5Widgets_DEFINITIONS})

# QT5_WRAP_UI(UIS_HDRS
# #  mainwindow.ui
# )

# qt5_add_resources(RESOURCE_ADDED resources.qrc)

# add_executable(grafips_c
#   main.cpp
#   data_sink.h
#   gripsrenderer.h
#   gripsrenderer.cpp
#   ${UIS_HDRS}
#   ${RESOURCE_ADDED} 
# )

#qt5_use_modules(grafips_c Core Gui Widgets OpenGL Quick Declarative) 

#target_link_libraries(grafips_c grafips_providers GL pthread)

add_subdirectory(test)
add_subdirectory(providers)
add_subdirectory(publishers)
add_subdirectory(subscriber)
add_subdirectory(graph)

