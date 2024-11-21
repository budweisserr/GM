# draw/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

set(DRAW_SOURCES
        drawing.cpp
        drawing_interface.cpp
)

set(DRAW_HEADERS
        drawing.h
        drawing_interface.h
)

add_library(draw STATIC ${DRAW_SOURCES} ${DRAW_HEADERS})
target_include_directories(draw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(draw PUBLIC model)