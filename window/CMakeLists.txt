# window/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

set(WINDOW_SOURCES
        mainwindow.cpp
)

set(WINDOW_HEADERS
        mainwindow.h
)

set(WINDOW_UI
        mainwindow.ui
)

add_library(window STATIC
        ${WINDOW_SOURCES}
        ${WINDOW_HEADERS}
        ${WINDOW_UI}
)

target_include_directories(window PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(window PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        draw
        model
)