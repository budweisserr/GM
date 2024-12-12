# model/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

set(MODEL_SOURCES
        point.cpp
        grid.cpp
        shape.cpp
        line.cpp
)

set(MODEL_HEADERS
        point.h
        grid.h
        shape.h
        line.h
)

add_library(model STATIC ${MODEL_SOURCES} ${MODEL_HEADERS})
target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})