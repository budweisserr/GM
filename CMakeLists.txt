cmake_minimum_required(VERSION 3.16)

project(GeometricTransformation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Add subdirectories first
add_subdirectory(model)
add_subdirectory(draw)
add_subdirectory(window)

# Create library targets for each subdirectory
set(MODEL_SOURCES
        model/point.cpp
        model/grid.cpp
        model/circle.cpp
        model/figure.cpp
        model/line.cpp
)

set(MODEL_HEADERS
        model/point.h
        model/grid.h
        model/circle.h
        model/figure.h
        model/line.h
)

set(DRAW_SOURCES
        draw/drawing.cpp
        draw/drawing_interface.cpp
)

set(DRAW_HEADERS
        draw/drawing.h
        draw/drawing_interface.h
)

set(WINDOW_SOURCES
        window/mainwindow.cpp
)

set(WINDOW_HEADERS
        window/mainwindow.h
)

set(WINDOW_UI
        window/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GeometricTransformation
            MANUAL_FINALIZATION
            main.cpp
            ${MODEL_SOURCES}
            ${MODEL_HEADERS}
            ${DRAW_SOURCES}
            ${DRAW_HEADERS}
            ${WINDOW_SOURCES}
            ${WINDOW_HEADERS}
            ${WINDOW_UI}
    )
else()
    if(ANDROID)
        add_library(GeometricTransformation SHARED
                main.cpp
                ${MODEL_SOURCES}
                ${MODEL_HEADERS}
                ${DRAW_SOURCES}
                ${DRAW_HEADERS}
                ${WINDOW_SOURCES}
                ${WINDOW_HEADERS}
                ${WINDOW_UI}
        )
    else()
        add_executable(GeometricTransformation
                main.cpp
                ${MODEL_SOURCES}
                ${MODEL_HEADERS}
                ${DRAW_SOURCES}
                ${DRAW_HEADERS}
                ${WINDOW_SOURCES}
                ${WINDOW_HEADERS}
                ${WINDOW_UI}
        )
    endif()
endif()

# Add include directories for all subdirectories
target_include_directories(GeometricTransformation PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/model
        ${CMAKE_CURRENT_SOURCE_DIR}/draw
        ${CMAKE_CURRENT_SOURCE_DIR}/window
)

target_link_libraries(GeometricTransformation PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GeometricTransformation)
endif()

set_target_properties(GeometricTransformation PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GeometricTransformation
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GeometricTransformation)
endif()